/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 17:31:12
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-19 20:47:51
 */

#pragma once

#include <map>
#include <stack>

#include "parallel.hpp"

class Runtime;
class Parallel;
class AstStatement;

using statments = std::vector<AstStatement*>;
using statments_table = std::map<std::string, statments>;

class AstStatement{
 public:
  virtual void run(Runtime *runtime, Parallel *thread) = 0;
};


class Context {
public:
  Context();
  Context(std::string block, size_t line, statments_table::iterator code);
  using symbol_table = std::map<std::string, std::string>;
  symbol_table _symbol_table;
  std::string _block;
  size_t _line;
  statments_table::iterator _code;
private:
};

class Runtime {
public:
  statments_table* _p_table;
  
  bool end;
  Context current;
  Runtime(statments_table* p_table);
  void jump(std::string block_name);
  void step(Parallel* thread);
  
private:
  std::stack<Context> _stack;
};
