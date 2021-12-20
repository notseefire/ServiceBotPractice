/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 17:31:16
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 10:16:59
 */

#include "runtime.hpp"

#include <string>
#include <boost/log/trivial.hpp>

Context::Context() {}

Context::Context(std::string block, size_t line, statments_table::iterator code)
    : _block(block), _line(line), _code(code) {}

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
