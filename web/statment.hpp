/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 20:00:53
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 14:53:32
 */

#pragma once

#include <string>

#include "parallel.hpp"
#include "runtime.hpp"

using namespace std;

class Set : public AstStatement {
 public:
  Set(string id, string init);
  void run(Runtime *runtime, Parallel *thread);

 private:
  string _id;
  string _init;
};

class Branch : public AstStatement {
 public:
  Branch(string id, string value, string proc_id);
  void run(Runtime *runtime, Parallel *thread);

 private:
  string _id;
  string _value;
  string _proc_id;
};

class Break : public AstStatement {
 public:
  void run(Runtime *runtime, Parallel *thread);
};

class Input : public AstStatement {
 public:
  explicit Input(string id, uint32_t wait_time = 5);
  void run(Runtime *runtime, Parallel *thread);

 private:
  string _id;
  uint32_t _wait_time;
};

class Print : public AstStatement {
 public:
  explicit Print(string id, string value);
  void run(Runtime *runtime, Parallel *thread);

 private:
  string _id;
  string _value;
};

class Assign : public AstStatement {
 public:
  Assign(string id, string value);
  void run(Runtime *runtime, Parallel *thread);

 private:
  string _id;
  string _value;
};
