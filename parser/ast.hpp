/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-11 14:42:59
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 00:56:14
 */

#pragma once

#include <string>
#include <vector>

#include "../web/statment.hpp"
#include "lexer.hpp"

using namespace std;

class Block {
 public:
  /**
   * @brief Construct a new Block object in a token stream.
   * search through whole stream and token between begin and end
   * will be contained into block
   * 
   * @param name  block name
   * @param begin  block start in token stream
   * @param end block end in token stream
   */
  Block(std::string name, Lexer::token_stream::iterator begin,
        Lexer::token_stream::iterator end);
  Input *find_input(Lexer::token_stream::iterator &begin,
                    Lexer::token_stream::iterator &end);
  Print *find_print(Lexer::token_stream::iterator &begin,
                    Lexer::token_stream::iterator &end);
  Set *find_set(Lexer::token_stream::iterator &begin,
                Lexer::token_stream::iterator &end);
  Branch *find_branch(Lexer::token_stream::iterator &begin,
                      Lexer::token_stream::iterator &end);

  statments _stmts;

 private:
  string _name;
};
