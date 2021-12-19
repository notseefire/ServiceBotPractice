/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 18:26:38
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-19 11:07:30
 */
#pragma once

#include <map>

#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"

using namespace std;

class Parser {
 public:
  using token_table = map<string, Block>;
  token_table parser(map<string, Lexer::token_stream> table);
  string find_proc(Lexer::token_stream::iterator &cur,
                   Lexer::token_stream::iterator &end);

 private:
  token_table _block_table;
};
