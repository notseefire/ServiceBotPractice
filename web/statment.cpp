/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 20:00:59
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 15:36:54
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
  BOOST_LOG_TRIVIAL(trace) << "branch(" << id << ", " << value << ", "
                           << proc_id << ")";
}

void Branch::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto it = current._symbol_table.find(_id);
  if (it == current._symbol_table.end()) {
    BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
    runtime->end = true;
  } else if (it->second == _value) {
    runtime->jump(_proc_id);
  }
}

Input::Input(string id, uint32_t wait_time) : _id(id), _wait_time(wait_time) {
  BOOST_LOG_TRIVIAL(trace) << "input(" << id << ")";
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
      BOOST_LOG_TRIVIAL(error) << "wait input timeout";
      runtime->end = true;
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

void Break::run(Runtime *runtime, Parallel *thread) { runtime->end = true; }

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