/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 21:07:58
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 17:41:10
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
  Lexer(unique_ptr<lookup_table> &keyword_table,
        unique_ptr<lookup_table> &operator_table);

  map<string, token_stream> lex(ScriptManager &manager);

 private:
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

  token_stream lex_script(const fs::path &script_path);
  bool lex_char(int& state, string &s);
};

class LookupTableFactory {
 public:
  unique_ptr<Lexer::lookup_table> get_keyword_table();
  unique_ptr<Lexer::lookup_table> get_operator_table();
};