/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 17:31:16
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-22 21:39:24
 */

#include "runtime.hpp"

#include <boost/log/trivial.hpp>
#include <string>
#include <optional>

void AstStatement::set_target(size_t target){
  _target = target;
}

size_t AstStatement::get_target() {
  return _target;
}

Context::Context() {}

Context::Context(std::string block, size_t line, statments_table::iterator code)
    : _block(block), _line(line), _code(code) {
      // initialize scope in Context
      add_scope();
}
  
std::optional<Context::symbol_table::iterator> Context::find_variable(std::string name) {
  for(auto reg = scope_stack.begin(); reg != scope_stack.end(); reg++) {
    auto it = reg->_symbol_table.find(name);
    if(it != reg->_symbol_table.end()) {
      return it;
    }
  }
  return std::nullopt;
}

std::list<Context::Register>::iterator Context::get_now_scope() {
  assert(!scope_stack.empty());
  return scope_stack.begin();
}

void Context::add_scope() {
  Register reg;
  reg.is_branch = false;
  scope_stack.push_front(std::move(reg));
}

void Context::pop_scope() {
  scope_stack.pop_front();
}

Runtime::Runtime(statments_table* p_table) : _p_table(p_table) {
  end = false;
  auto it = _p_table->find(std::string("main"));
  if (it == _p_table->end()) {
    end = true;
    BOOST_LOG_TRIVIAL(error) << "can't find main entry";
  } else {
    current = Context("main", 0, it);
  }
}

void Runtime::jump(std::string block_name) {
  _stack.push(current);
  auto it = _p_table->find(std::string(block_name));
  if (it == _p_table->end()) {
    end = true;
    BOOST_LOG_TRIVIAL(error) << block_name << " is undefined";
  } else {
    if (it->second.size() == 0) {
      end = true;
    }
    current = Context(block_name, 0, it);
  }
}

void Runtime::step(Parallel* thread) {
  statments block = current._code->second;
  while (current._line == block.size()) {
    if (_stack.empty()) {
      end = true;
      return;
    } else {
      current = _stack.top();
      block = current._code->second;
      _stack.pop();
    }
  }

  auto action = block[current._line];
  current._line++;
  action->run(this, thread);
}
