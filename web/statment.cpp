/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 20:00:59
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-23 10:14:10
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
  if (!it.has_value()) {
    BOOST_LOG_TRIVIAL(trace) << "declare " << _id << " = " << _init;
    current.get_now_scope()->_symbol_table[_id] = _init;
  } else {
    BOOST_LOG_TRIVIAL(error) << "redefine variable is unacceptive";
    runtime->end = true;
  }
}

Branch::Branch(string id, string value, string proc_id, bool check)
    : _id(id), _value(value), _proc_id(proc_id), _check(check) {
  if (check) {
    BOOST_LOG_TRIVIAL(trace) << "branch(" << proc_id << ")";
  } else {
    BOOST_LOG_TRIVIAL(trace) << "branch(" << id << ", " << value << ", "
                            << proc_id << ")";
  }
}

void Branch::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto it = current.find_variable(_id);
  if(!_check) {
    runtime->jump(_proc_id);
    return;
  }

  if (!it.has_value()) {
    BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
    runtime->end = true;
  } else {
    if (it.value()->second == _value) {
      BOOST_LOG_TRIVIAL(trace) << "branch hit";
      current.get_now_scope()->is_branch = true;
      runtime->jump(_proc_id);
    } else {
      BOOST_LOG_TRIVIAL(trace) << "branch not hit " << it.value()->first << " != " << _value;
    } 
  } 
}

Input::Input(string id, uint32_t wait_time) : _id(id), _wait_time(wait_time) {
  BOOST_LOG_TRIVIAL(trace) << "input(" << id << ", " << wait_time << ")";
}

void Input::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto it = current.find_variable(_id);
  if (!it.has_value()) {
    BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
    runtime->end = true;
  } else {
    auto data = thread->wait_data(_wait_time * 1000);
    if (data.has_value()) {
      it.value()->second = data->get_inner();
      BOOST_LOG_TRIVIAL(trace) << "input " << it.value()->second;
    } else {
      BOOST_LOG_TRIVIAL(warning) << "wait input timeout";
      it.value()->second = "";
    }
  }
}

Print::Print(string id, vector<Token> value_list) : _id(id), _value_list(value_list) {
  string msg = "print(";
  for(auto index = _value_list.begin(); index != _value_list.end(); index++) {
    if(index->is_string()) {
      msg += "\"" + index->get_string() + "\"";
    } else {
      msg += index->get_id()._value;
    }
    if(_value_list.end() - index != 1) {
      msg += ", ";
    }
  }
  msg += ")";
  BOOST_LOG_TRIVIAL(trace) << msg;
}

void Print::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  string msg;
  for(auto index = _value_list.begin(); index != _value_list.end(); index++) {
    if(index->is_string()) {
      msg += index->get_string();
    } else {
      auto it = current.find_variable(index->get_id()._value);
      if (!it.has_value()) {
        BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
        runtime->end = true;
        return;
      } else {
        msg += it.value()->second;
      }
    }
  }
  thread->send_private_msg(msg);
}

void Break::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto loop_target = current._code->second[get_target()];
  current._line = loop_target->get_target();
  current.pop_scope();
}

Assign::Assign(string id, vector<Token> value_list) : _id(id), _value_list(value_list) {
  string msg = "assign(" + id + " = ";
  for(auto index = _value_list.begin(); index != _value_list.end(); index++) {
    if(index->is_string()) {
      msg += "\"" + index->get_string() + "\"";
    } else {
      msg += index->get_id()._value;
    }
    if(_value_list.end() - index != 1) {
      msg += ", ";
    }
  }
  msg += ")";
  BOOST_LOG_TRIVIAL(trace) << msg;
}

void Assign::run(Runtime *runtime, Parallel *thread) {
  Context &current = runtime->current;
  auto it = current.find_variable(_id);
  if (!it.has_value()) {
    BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
    runtime->end = true;
    return;
  }
  string assign_value = "";
  for(auto index = _value_list.begin(); index != _value_list.end(); index++) {
    if(index->is_string()) {
      assign_value += index->get_string();
    } else {
      auto temp = current.find_variable(index->get_id()._value);
      if (!temp.has_value()) {
        BOOST_LOG_TRIVIAL(error) << "the variable " << _id << " have not declared";
        runtime->end = true;
        return;
      } else {
        assign_value += temp.value()->second;
      }
    }
  }
  it.value()->second = assign_value;
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
  if(!current.get_now_scope()->is_branch) {
    current._line = get_target();
  }
  current.pop_scope();
}