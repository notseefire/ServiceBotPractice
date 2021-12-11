/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 18:26:38
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-11 14:59:38
 */
#pragma once

#include <map>

#include "token.hpp"
#include "lexer.hpp"
#include "ast.hpp"

using namespace std;

class Parser {
 public:
  using token_table = map<string, Block>;
  Parser(map<string, Lexer::token_stream> table);

 private:

};
