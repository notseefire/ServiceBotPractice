/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-11 14:43:02
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-11 17:00:49
 */

#pragma once

#include <string>

#include "ast.hpp"
#include "token.hpp"

using namespace std;

Set::Set(string id, string init): _id(id), _init(init) {

}


Branch::Branch(string id, string value, string proc_id): _id(id), _value(value), _proc_id(proc_id) {

}

Input::Input(string id): _id(id) {

}

Echo::Echo(string value): _value(value) {

}

Block::Block(std::string name, Lexer::token_stream::iterator begin, Lexer::token_stream::iterator end) {
  while(begin != end) {
    if(!begin->is_reserved_token()) {
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
      begin++;
      _stmts.push_back(make_shared<Break>(Break()));

    break;

    case reserved_token::ECHO:
      begin++;
      _stmts.push_back(find_echo(begin, end));
    break;

    case reserved_token::INPUT:
      begin++;
      _stmts.push_back(find_input(begin, end));
    break;

    default:
      throw "unexpected token";
    }
  }
}

shared_ptr<Set> Block::find_set(Lexer::token_stream::iterator &begin, Lexer::token_stream::iterator &end) {
  string id;
  string init;
  if(!begin->is_identifier()) {
    throw "expected identifier";
  }

  id = begin->get_id()._value;
  begin++;

  if(begin == end) {
    return make_shared<Set>(Set(id, init));
  }
  if(!(begin->is_reserved_token() && begin->get_token() == reserved_token::ASSIGN)) {
    return make_shared<Set>(Set(id, init));
  }

  begin++;

  if(begin == end) {
    throw "expect init value";
  }
  if(!(begin->is_string())) {
    throw "expect string value";
  }

  init = begin->get_string();
  return make_shared<Set>(Set(id, init));
}

shared_ptr<Branch> Block::find_branch(Lexer::token_stream::iterator &begin, Lexer::token_stream::iterator &end) {
  string id;
  string condition;
  string procedure;
  if(end - begin < 3) {
    throw "unexpected call operation";
  }

  if(!begin->is_identifier()) {
    throw "expect identifier";
  }

  id = begin->get_id()._value;
  begin++;

  if(!begin->is_string()) {
    throw "expect condition value";
  }

  condition = begin->get_string();
  begin++;

  if(!begin->is_identifier()) {
    throw "expect procedure name";
  }

  procedure = begin->get_id()._value;

  return make_shared<Branch>(Branch(id, condition, procedure));
}

shared_ptr<Input> find_input(Lexer::token_stream::iterator &begin, Lexer::token_stream::iterator &end) {
  string id;
  if(begin == end) {
    throw "expect identifer";
  }

  if(!begin->is_identifier()) {
    throw "expect identifer";
  }

  id = begin->get_id()._value;
  return make_shared<Input>(Input(id));
}

shared_ptr<Echo> find_echo(Lexer::token_stream::iterator &begin, Lexer::token_stream::iterator &end) {
  string value;
  if(begin == end) {
    throw "expect string value";
  }

  if(!begin->is_string()) {
    throw "expect string value";
  }

  id = begin->get_string();
  return make_shared<Input>(Echo(value));
}