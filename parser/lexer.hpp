/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 21:07:58
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 00:50:06
 */

#pragma once

#include <map>
#include <memory>
#include <vector>

#include "script.hpp"
#include "token.hpp"

using namespace std;

class Lexer {
 public:
  using lookup_table = map<string, reserved_token>;
  using token_stream = vector<Token>;

  /**
   * @brief Construct a new Lexer object
   * 
   * @param keyword_table  lookup_table for keyword
   * @param operator_table  lookup_table for operator
   */
  Lexer(unique_ptr<lookup_table> &keyword_table,
        unique_ptr<lookup_table> &operator_table);

  /**
   * @brief lex through script manager, analyze all script and generate token stream
   * one by one
   * 
   * @param manager a `_container` to script file path
   * @return map<string, token_stream>  map from script name to token_stream
   */
  map<string, token_stream> lex(ScriptManager &manager);

 private:

 /**
  * @brief find escape character, such as n, it will return \\n
  * as a result
  * 
  * @return char found escape character.
  */
  char transfer();

  unique_ptr<lookup_table> _keyword_table, _operator_table;
  map<string, token_stream> _token_table;
  char _ch;
  lookup_table::iterator _it;
  bool _is_alpha;
  bool _is_digit;
  bool _is_operator;
  bool _comment_line;
  size_t _line_num, _number, _cur_number;
  token_stream _stream;

  /**
   * @brief Analysis script into token stream, it deal with only one script one time.
   * 
   * @param script_path one script file path
   * @return token_stream
   */
  token_stream lex_script(const fs::path &script_path);

  /**
   * @brief Lexer automation, call the function will change the state in automation
   * 
   * @param state current state
   * @param s string with one character, it deal with string more effectively and easy.
   * @return true lexer should continue its analysis instead of receive next one character.
   */
  bool lex_char(int &state, string &s);
};

class LookupTableFactory {
 public:
  unique_ptr<Lexer::lookup_table> get_keyword_table();
  unique_ptr<Lexer::lookup_table> get_operator_table();
};