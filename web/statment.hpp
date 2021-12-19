/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 20:00:53
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-19 20:21:19
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
  Input(string id);
  void run(Runtime *runtime, Parallel *thread);

 private:
  string _id;
};

class Print : public AstStatement {
 public:
  Print(string value);
  void run(Runtime *runtime, Parallel *thread);

 private:
  string _value;
};
