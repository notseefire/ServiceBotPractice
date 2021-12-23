/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 20:00:53
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-23 10:02:03
 */

#pragma once

#include <string>
#include <vector>

#include "parallel.hpp"
#include "runtime.hpp"
#include "../parser/token.hpp"

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
  Branch(string id, string value, string proc_id, bool check);
  void run(Runtime *runtime, Parallel *thread);

 private:
  bool _check;
  string _id;
  string _value;
  string _proc_id;
};

class Break : public AstStatement {
 public:
  void run(Runtime *runtime, Parallel *thread);

 private:
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
  explicit Print(string id, vector<Token> value_list);
  void run(Runtime *runtime, Parallel *thread);

 private:
  string _id;
  vector<Token> _value_list;
};

class Assign : public AstStatement {
 public:
  Assign(string id, vector<Token> value_list);
  void run(Runtime *runtime, Parallel *thread);

 private:
  string _id;
  vector<Token> _value_list;
};

class Other : public AstStatement {
 public:
  Other(string id);
  void run(Runtime *runtime, Parallel *thread);

 private:
  string _id;
};

class LoopBegin : public AstStatement {
 public:
  LoopBegin();
  void run(Runtime *runtime, Parallel *thread);

 private:
};

class LoopEnd : public AstStatement {
 public:
  LoopEnd();
  void run(Runtime *runtime, Parallel *thread) override;

 private:
};