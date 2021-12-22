/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 20:00:59
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-22 19:40:05
 */

#include "statment.hpp"

#include <boost/log/trivial.hpp>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>

using namespace std;

Set::Set(string id, string init) : _id(id), _init(init) {
  BOOST_LOG_TRIVIAL(trace) << "set(" << id << ", " << init << ")";
}

void Set::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  if (current._symbol_table.find(_id) == current._symbol_table.end()) {
    current._symbol_table[_id] = _init;
  } else {
    BOOST_LOG_TRIVIAL(error) << "redefine variable is unacceptive";
    runtime->end = true;
  }
}

Branch::Branch(string id, string value, string proc_id)
    : _id(id), _value(value), _proc_id(proc_id) {
  if (value.empty()) {
    BOOST_LOG_TRIVIAL(trace) << "branch(" << proc_id << ")";
  } else {
    BOOST_LOG_TRIVIAL(trace) << "branch(" << id << ", " << value << ", "
                            << proc_id << ")";
  }
}

void Branch::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto it = current._symbol_table.find(_id);
  if (it == current._symbol_table.end()) {
    BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
    runtime->end = true;
  } else if (it->second == _value) {
    current._is_branch = true;
    runtime->jump(_proc_id);
  }
}

Input::Input(string id, uint32_t wait_time) : _id(id), _wait_time(wait_time) {
  BOOST_LOG_TRIVIAL(trace) << "input(" << id << ", " << wait_time << ")";
}

void Input::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto it = current._symbol_table.find(_id);
  if (it == current._symbol_table.end()) {
    BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
    runtime->end = true;
  } else {
    auto data = thread->wait_data(_wait_time * 1000);
    if (data.has_value()) {
      it->second = data->get_inner();
    } else {
      BOOST_LOG_TRIVIAL(warning) << "wait input timeout";
      it->second = "";
    }
  }
}

Print::Print(string id, string value) : _id(id), _value(value) {
  if (id.empty()) {
    BOOST_LOG_TRIVIAL(trace) << "print(\"" << value << "\")";
  } else {
    BOOST_LOG_TRIVIAL(trace) << "print(" << id << ")";
  }
}

void Print::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  if(_id.empty()) {
    thread->send_private_msg(_value);
  } else {
    auto it = current._symbol_table.find(_id);
    if (it == current._symbol_table.end()) {
      BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
      runtime->end = true;
    } else {
      thread->send_private_msg(it->second);
    }
  }
}

void Break::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto loop_target = current._code->second[get_target()];
  current._line = loop_target->get_target();
}

Assign::Assign(string id, string value) : _id(id), _value(value) {
  BOOST_LOG_TRIVIAL(trace) << "assign(" << id << ", " << value <<")";
}

void Assign::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto it = current._symbol_table.find(_id);
  if (it != current._symbol_table.end()) {
    it->second = _value;
  } else {
    BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
    runtime->end = true;
  }
}

Other::Other(string id) : _id(id) {
  BOOST_LOG_TRIVIAL(trace) << "other(" << id << ")";
}

void Other::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  if(!current._is_branch) {
    runtime->jump(_id);
  }
}

LoopBegin::LoopBegin(){
  BOOST_LOG_TRIVIAL(trace) << "LoopBegin";
}

void LoopBegin::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
}

LoopEnd::LoopEnd() {
  BOOST_LOG_TRIVIAL(trace) << "LoopEnd";
}

void LoopEnd::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  current._line = get_target();
}