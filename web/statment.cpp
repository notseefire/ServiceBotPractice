/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 20:00:59
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-22 21:32:37
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
  auto it = current.find_variable(_id);
  if (!it.first) {
    BOOST_LOG_TRIVIAL(trace) << "declare " << _id << " = " << _init;
    current.get_now_scope()->_symbol_table[_id] = _init;
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
  auto it = current.find_variable(_id);
  if (!it.first) {
    BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
    runtime->end = true;
  } else {
    auto it_table = it.second;
    if (it_table->second == _value) {
      BOOST_LOG_TRIVIAL(trace) << "branch hit";
      current.get_now_scope()->is_branch = true;
      runtime->jump(_proc_id);
    } else {
      BOOST_LOG_TRIVIAL(trace) << "branch not hit " << it_table->second << " != " << _value;
    } 
  } 
}

Input::Input(string id, uint32_t wait_time) : _id(id), _wait_time(wait_time) {
  BOOST_LOG_TRIVIAL(trace) << "input(" << id << ", " << wait_time << ")";
}

void Input::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto it = current.find_variable(_id);
  if (!it.first) {
    BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
    runtime->end = true;
  } else {
    auto data = thread->wait_data(_wait_time * 1000);
    auto it_table = it.second;
    if (data.has_value()) {
      it_table->second = data->get_inner();
      BOOST_LOG_TRIVIAL(trace) << "input " << it_table->second;
    } else {
      BOOST_LOG_TRIVIAL(warning) << "wait input timeout";
      it_table->second = "";
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
    auto it = current.find_variable(_id);
    if (!it.first) {
      BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
      runtime->end = true;
    } else {
      auto it_table = it.second;
      thread->send_private_msg(it_table->second);
    }
  }
}

void Break::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto loop_target = current._code->second[get_target()];
  current._line = loop_target->get_target();
  current.pop_scope();
}

Assign::Assign(string id, string value) : _id(id), _value(value) {
  BOOST_LOG_TRIVIAL(trace) << "assign(" << id << ", " << value <<")";
}

void Assign::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto it = current.find_variable(_id);
  if (it.first) {
    auto it_table = it.second; 
    it_table->second = _value;
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
  if(!current.get_now_scope()->is_branch) {
    runtime->jump(_id);
  }
}

LoopBegin::LoopBegin(){
  BOOST_LOG_TRIVIAL(trace) << "LoopBegin";
}

void LoopBegin::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  current.add_scope();
}

LoopEnd::LoopEnd() {
  BOOST_LOG_TRIVIAL(trace) << "LoopEnd";
}

void LoopEnd::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  current._line = get_target();
  current.pop_scope();
}