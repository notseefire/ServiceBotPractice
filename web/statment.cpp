/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 20:00:59
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-19 21:46:21
 */

#include <boost/log/trivial.hpp>
#include <string>

#include "statment.hpp"

using namespace std;

Set::Set(string id, string init) : _id(id), _init(init) {
  BOOST_LOG_TRIVIAL(trace) << "set("<<id<<", " << init << ")";
}

void Set::run(Runtime *runtime, Parallel *thread) {
  Context& current = runtime->current;
  if(current._symbol_table.find(_id) == current._symbol_table.end()) {
    current._symbol_table[_id] = _init;
  } else {
    BOOST_LOG_TRIVIAL(error) << "redefine variable is unacceptive" ;
    runtime->end = true;
  }
}

Branch::Branch(string id, string value, string proc_id)
    : _id(id), _value(value), _proc_id(proc_id) {
  BOOST_LOG_TRIVIAL(trace) << "branch("<<id<<", " << value << ", " << proc_id << ")";
}

void Branch::run(Runtime *runtime, Parallel *thread) { 
  Context& current = runtime->current;
  auto it = current._symbol_table.find(_id);
  if(it == current._symbol_table.end()) {
    runtime->end = true; 
  } else if(it->second == _value) {
    runtime->jump(_proc_id);
  }
}

Input::Input(string id) : _id(id) {
  BOOST_LOG_TRIVIAL(trace) << "input("<<id <<")";
}

void Input::run(Runtime *runtime, Parallel *thread) {
  Context& current = runtime->current;
  auto it = current._symbol_table.find(_id);
  if(it == current._symbol_table.end()) {
    runtime->end = true;
  } else {
    QMessage data = thread->wait_data();
    it->second = data.get_inner();
  }
}

Print::Print(string value) : _value(value) {
  BOOST_LOG_TRIVIAL(trace) << "print("<<value <<")";
}

void Print::run(Runtime *runtime, Parallel *thread) {
  thread->send_private_msg(_value);
}

void Break::run(Runtime *runtime, Parallel *thread) {
  runtime->end = true;
}
