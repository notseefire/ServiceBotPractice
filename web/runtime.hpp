/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 17:31:12
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-23 11:18:00
 */

#pragma once

#include <map>
#include <stack>
#include <string>
#include <vector>
#include <optional>

#include "parallel.hpp"

class Runtime;
class Parallel;
class AstStatement;

using statments = std::vector<AstStatement*>;
using statments_table = std::map<std::string, statments>;

/**
 * @brief `AstStatment` is an interface to run statment. it is a
 * abstract class.
 *
 */
class AstStatement {
 public:
  virtual void run(Runtime* runtime, Parallel* thread) = 0;
  void set_target(size_t target);
  size_t get_target();
 private:
  size_t _target;
};

/**
 * @brief  `Context` describe the situation run in a block, when one `Context`
 * finish, it will switch to previous `Context` if this block branch to fnished
 * block before. Context is defined as position, symbol table and code body
 *
 */
class Context {
 public:
  Context();
  Context(std::string block, size_t line, statments_table::iterator code);
  using symbol_table = std::map<std::string, std::string>;

  struct Register {
    symbol_table _symbol_table;
    bool is_branch;
  };

  std::optional<symbol_table::iterator> find_variable(std::string name);
  void add_scope();
  void pop_scope();
  std::list<Register>::iterator get_now_scope();

  std::list<Register> scope_stack;
  std::string _block;
  size_t _line;
  statments_table::iterator _code;

 private:
};

/**
 * @brief  Runtime can be described as a stack of `Context` and switch between
 * them. server thread run script through `Runtime` by call `step` method.
 * `Runtime` also describe the status of running by bool value.
 *
 */
class Runtime {
 public:
  statments_table* _p_table;

  bool end;
  Context current;
  /**
   * @brief Construct a new Runtime object
   *
   * @param p_table
   */
  explicit Runtime(statments_table* p_table);

  /**
   * @brief
   *
   * @param block_name
   */
  void jump(std::string block_name);

  /**
   * @brief
   *
   * @param thread
   */
  void step(Parallel* thread);

 private:
  std::stack<Context> _stack;
};
