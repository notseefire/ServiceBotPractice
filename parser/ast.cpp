/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-11 14:43:02
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 15:53:00
 */

#include "ast.hpp"

#include <boost/log/trivial.hpp>
#include <string>

#include "../web/statment.hpp"
#include "token.hpp"

using namespace std;
Set *Block::find_set(Lexer::token_stream::iterator &begin,
                     const Lexer::token_stream::iterator &end) const {
  string id;
  string init;
  if (!begin->is_identifier()) {
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
  if (end - begin < 4) {
    throw "unexpected call operation";
  }

  if (!begin->is_identifier()) {
    throw "expect identifier";
  }

  id = begin->get_id()._value;
  begin++;

  if (!begin->is_reserved_token() ||
      begin->get_token() != reserved_token::ASSIGN) {
    throw "expect assign operator in branch";
  }

  begin++;

  if (!begin->is_string()) {
    throw "expect condition value";
  }

  condition = begin->get_string();
  begin++;

  if (!begin->is_identifier()) {
    throw "expect procedure name";
  }

  procedure = begin->get_id()._value;
  begin++;

  return new Branch(id, condition, procedure);
}

Input *Block::find_input(Lexer::token_stream::iterator &begin,
                         const Lexer::token_stream::iterator &end) const {
  string id;
  if (begin == end) {
    throw "expect identifer";
  }

  if (!begin->is_identifier()) {
    throw "expect identifer";
  }

  id = begin->get_id()._value;
  begin++;
  return new Input(id);
}

Print *Block::find_print(Lexer::token_stream::iterator &begin,
                         const Lexer::token_stream::iterator &end) const {
  string id;
  string value;
  if (begin == end || !(begin->is_string() || begin->is_identifier())) {
    throw "expect string value";
  }

  if (begin->is_identifier()) {
    id = begin->get_id()._value;
  } else {
    value = begin->get_string();
  }
  begin++;

  return new Print(id, value);
}

Assign *Block::find_assign(Lexer::token_stream::iterator &begin,
                           const Lexer::token_stream::iterator &end) const {
  string id;
  string value;
  if (!begin->is_identifier()) {
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

  if (begin == end) {
    throw "expect init value";
  }
  if (!(begin->is_string())) {
    throw "expect string value";
  }

  value = begin->get_string();
  begin++;
  return new Assign(id, value);
}

Block::Block(std::string name, Lexer::token_stream::iterator begin,
             Lexer::token_stream::iterator end) {
  while (begin != end) {
    if (!begin->is_reserved_token()) {
      throw "expect a key word";
    }
    reserved_token token = begin->get_token();
    switch (token) {
      case reserved_token::SET:
        begin++;
        _stmts.push_back(find_set(begin, end));

        BOOST_LOG_TRIVIAL(trace) << "adding a set segment";
        break;

      case reserved_token::CALL:
        begin++;
        _stmts.push_back(find_branch(begin, end));

        BOOST_LOG_TRIVIAL(trace) << "adding a branch segment";

        break;

      case reserved_token::BREAK:
        begin++;
        _stmts.push_back(new Break());

        BOOST_LOG_TRIVIAL(trace) << "adding a break segment";

        break;

      case reserved_token::ECHO:
        begin++;
        _stmts.push_back(find_print(begin, end));
        BOOST_LOG_TRIVIAL(trace) << "adding a print segment";

        break;

      case reserved_token::INPUT:
        begin++;
        _stmts.push_back(find_input(begin, end));
        BOOST_LOG_TRIVIAL(trace) << "adding a input segment";

        break;

      case reserved_token::CHANGE:
        begin++;
        _stmts.push_back(find_assign(begin, end));
        BOOST_LOG_TRIVIAL(trace) << "adding a input segment";

        break;

      default:
        std::cout << static_cast<int>(token) << std::endl;
        throw "unexpected token";
    }
  }
}
