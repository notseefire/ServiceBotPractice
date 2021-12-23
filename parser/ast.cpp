/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-11 14:43:02
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-23 12:07:26
 */

#include "token.hpp"
#include "ast.hpp"
#include <boost/log/trivial.hpp>
#include <string>

#include "../web/statment.hpp"

using namespace std;
Set *Block::find_set(Lexer::token_stream::iterator &begin,
                     const Lexer::token_stream::iterator &end) const {
  string id;
  string init;
  if (begin == end || !begin->is_identifier()) {
    throw "expected identifier";
  }

  id = begin->get_id()._value;
  begin++;

  if (begin == end) {
    return new Set(id, init);
  }
  if (!(begin->is_reserved_token() &&
        begin->get_token() == reserved_token::ASSIGN)) {
    return new Set(id, init);
  }

  begin++;

  if (begin == end) {
    throw "expect init value";
  }
  if (!(begin->is_string())) {
    throw "expect string value";
  }

  init = begin->get_string();
  begin++;

  return new Set(id, init);
}

Branch *Block::find_branch(Lexer::token_stream::iterator &begin,
                           const Lexer::token_stream::iterator &end) const {
  string id;
  string condition;
  string procedure;

  if (begin == end || !begin->is_identifier()) {
    throw "expect identifier";
  }

  id = begin->get_id()._value;
  begin++;

  // now id is procedure name without conditino
  if (!begin->is_reserved_token() ||
      begin->get_token() != reserved_token::ASSIGN) {
    return new Branch(id, condition, id, false);
  }

  begin++;

  if (begin == end || !begin->is_string()) {
    throw "expect condition value";
  }

  condition = begin->get_string();
  begin++;

  if (begin == end || !begin->is_identifier()) {
    throw "expect procedure name";
  }

  procedure = begin->get_id()._value;
  begin++;

  return new Branch(id, condition, procedure, true);
}

Input *Block::find_input(Lexer::token_stream::iterator &begin,
                         const Lexer::token_stream::iterator &end) const {
  string id;
  int wait_time;
  if (begin == end || !begin->is_identifier()) {
    throw "expect identifer";
  }

  id = begin->get_id()._value;
  begin++;

  if (begin == end) {
    return new Input(id);
  }

  if (!begin->is_reserved_token() || begin->get_token() != reserved_token::WAIT) {
    return new Input(id);
  }
  begin++;

  if (begin == end || !begin->is_number()) {
    throw "expect wait number";
  }

  wait_time = begin->get_number();
  begin++;

  return new Input(id, wait_time);
}

Print *Block::find_print(Lexer::token_stream::iterator &begin,
                         const Lexer::token_stream::iterator &end) const {
  string id;
  vector<Token> value_list;
  if (begin == end || !(begin->is_string() || begin->is_identifier())) {
    throw "expect output value";
  }
  value_list.push_back(*begin);
  begin++;
  while(begin != end && begin->is_reserved_token() && begin->get_token() == reserved_token::OP_ADD) {
    begin++;

    if (begin == end || !(begin->is_string() || begin->is_identifier())) {
      throw "expect value after add operator";
    }

    value_list.push_back(*begin);

    begin++;
  }

  return new Print(id, value_list);
}

Assign *Block::find_assign(Lexer::token_stream::iterator &begin,
                           const Lexer::token_stream::iterator &end) const {
  string id;
  if (begin == end || !begin->is_identifier()) {
    throw "expected identifier";
  }

  id = begin->get_id()._value;
  begin++;

  if (begin == end) {
    throw "expect assign operator";
  }
  if (!begin->is_reserved_token() ||
      begin->get_token() != reserved_token::ASSIGN) {
    throw "expect assign operator";
  }

  begin++;

  vector<Token> value_list;
  if (begin == end || !(begin->is_string() || begin->is_identifier())) {
    throw "expect init value";
  }
  value_list.push_back(*begin);
  begin++;
  while(begin != end && begin->is_reserved_token() && begin->get_token() == reserved_token::OP_ADD) {
    begin++;

    if (begin == end || !(begin->is_string() || begin->is_identifier())) {
      throw "expect value after add operator";
    }

    value_list.push_back(*begin);

    begin++;
  }

  return new Assign(id, value_list);
}

Other *Block::find_other(Lexer::token_stream::iterator &begin,
                    const Lexer::token_stream::iterator &end) const {
  string id;
  if (begin == end || !begin->is_identifier()) {
    throw "expected identifier";
  }
  id = begin->get_id()._value;
  begin++;
  return new Other(id);
}

Load *Block::find_load(Lexer::token_stream::iterator &begin,
                     const Lexer::token_stream::iterator &end) const {
  string id;
  string init;
  if (begin == end || !begin->is_identifier()) {
    throw "expected identifier";
  }

  id = begin->get_id()._value;
  begin++;

  if (begin == end) {
    throw "expect data name";
  }
  if (!(begin->is_string())) {
    throw "expect data name";
  }

  init = begin->get_string();
  begin++;

  return new Load(id, init);
}

Store *Block::find_store(Lexer::token_stream::iterator &begin,
                     const Lexer::token_stream::iterator &end) const {
  string id;
  string init;
  if (begin == end || !begin->is_identifier()) {
    throw "expected identifier";
  }

  id = begin->get_id()._value;
  begin++;

  if (begin == end) {
    throw "expect data name";
  }
  if (!(begin->is_string())) {
    throw "expect data name";
  }

  init = begin->get_string();
  begin++;

  return new Store(id, init);
}


Block::Block(std::string name, Lexer::token_stream::iterator begin,
             Lexer::token_stream::iterator end) {
  size_t line = 0;
  std::stack<std::pair<LoopBegin*, size_t>> loop_stack;
  int if_stack_tot = 0;
  while (begin != end) {
    try {
      
      if (!begin->is_reserved_token()) {
        throw "expect a key word";
      }
      reserved_token token = begin->get_token();
      switch (token) {
        case reserved_token::SET:
          begin++;
          _stmts.push_back(find_set(begin, end));

          break;

        case reserved_token::CALL:
          begin++;
          _stmts.push_back(find_branch(begin, end));


          break;

        case reserved_token::BREAK:
          if(loop_stack.empty()) throw "unexpected break";
          begin++;
          {
            auto break_temp = new Break();
            break_temp->set_target(loop_stack.top().second);
            _stmts.push_back(break_temp);
          }


          break;

        case reserved_token::LOOPBEGIN:
          begin++;
          {
            auto loop_begin_temp = new LoopBegin();
            _stmts.push_back(loop_begin_temp);
            loop_stack.push(std::make_pair(loop_begin_temp, line));
          }

          break;

        case reserved_token::LOOPEND:
          if(loop_stack.empty()) throw "unexpected endloop";
          begin++;
          {
            auto loop_end_temp = new LoopEnd();
            auto loop_begin = loop_stack.top();
            loop_begin.first->set_target(line + 1);
            loop_end_temp->set_target(loop_begin.second);
            loop_stack.pop();

            _stmts.push_back(loop_end_temp);
          }

          break;

        case reserved_token::ECHO:
          begin++;
          _stmts.push_back(find_print(begin, end));

          break;

        case reserved_token::INPUT:
          begin++;
          _stmts.push_back(find_input(begin, end));

          break;

        case reserved_token::CHANGE:
          begin++;
          _stmts.push_back(find_assign(begin, end));

          break;

        case reserved_token::OTHER:
          begin++;
          _stmts.push_back(find_other(begin, end));

          break;
        case reserved_token::LOAD:
          begin++;
          _stmts.push_back(find_load(begin, end));

          break;

        case reserved_token::STORE:
          begin++;
          _stmts.push_back(find_store(begin, end));

          break;
        default:
          std::cout << static_cast<int>(token) << std::endl;
          throw "unexpected token";
      }
      line++;
    } catch(const char* msg) {
      BOOST_LOG_TRIVIAL(error) << "(" << begin->_line << ", " << begin->_number << ")" << msg;
      throw "parser error";
    }
  }
  if(!loop_stack.empty()) {
    BOOST_LOG_TRIVIAL(error) << "loop no end";
    throw "parser error";
  }
}
