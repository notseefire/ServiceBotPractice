/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 18:26:38
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 15:53:09
 */
#pragma once

#include <map>
#include <string>

#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"

using namespace std;

/**
 * @brief Parser is generator of AST with token stream input.
 * My script can be divided into many blocks and every block stands for
 * several code segments.
 *
 */
class Parser {
 public:
  using token_table = map<string, statments>;

  /**
   * @brief parser token stream into AST
   *
   * @param table token stream with script file name
   * @return token_table map<string, statments>, string is name of block, and
   * statments are concrete code segments
   */
  token_table parser(map<string, Lexer::token_stream> table);

  /**
   * @brief find block name start from `cur`. When `cur` isn't
   * block start, it throw exception
   *
   * @param cur where block name should start
   * @param end script end
   * @return string  name of block
   */
  string find_proc(Lexer::token_stream::iterator &cur,
                   Lexer::token_stream::iterator &end);

 private:
  token_table _block_table;
};
