/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-11 14:42:59
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-19 10:32:12
 */

#pragma once

#include <string>
#include <vector>

#include "lexer.hpp"

using namespace std;

class AstStatement {
 public:
};

class Set : public AstStatement {
 public:
  Set(string id, string init);

 private:
  string _id;
  string _init;
};

class Branch : public AstStatement {
 public:
  Branch(string id, string value, string proc_id);

 private:
  string _id;
  string _value;
  string _proc_id;
};

class Break : public AstStatement {
 public:
};

class Input : public AstStatement {
 public:
  Input(string id);

 private:
  string _id;
};

class Print : public AstStatement {
 public:
  Print(string value);

 private:
  string _value;
};

class Block {
 public:
  Block(std::string name, Lexer::token_stream::iterator begin,
        Lexer::token_stream::iterator end);
  shared_ptr<Input> find_input(Lexer::token_stream::iterator &begin,
                               Lexer::token_stream::iterator &end);
  shared_ptr<Print> find_print(Lexer::token_stream::iterator &begin,
                             Lexer::token_stream::iterator &end);
  shared_ptr<Set> find_set(Lexer::token_stream::iterator &begin,
                           Lexer::token_stream::iterator &end);
  shared_ptr<Branch> find_branch(Lexer::token_stream::iterator &begin,
                                 Lexer::token_stream::iterator &end);

 private:
  string _name;
  vector<shared_ptr<AstStatement>> _stmts;
};
