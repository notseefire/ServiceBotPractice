/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 21:07:50
 * @LastEditors: CYKS
 * @LastEditTime: 2021-11-30 15:56:20
 */

#include "lexer.hpp"

#include <fstream>
#include <string>
#include <utility>

#include "token.hpp"

using namespace std;

Lexer::Lexer(unique_ptr<lookup_table> &keyword_table,
             unique_ptr<lookup_table> &operator_table)
    : _keyword_table(move(keyword_table)),
      _operator_table(move(operator_table)) {}

map<string, Lexer::token_stream> Lexer::lex(ScriptManager &manager) {
  for (auto t = manager.begin(); t != manager.end(); t++) {
    Lexer::token_stream stream = lex_script((*t).second);
    _token_table[static_cast<string>((*t).second)] = stream;
  }
  return _token_table;
}

Lexer::token_stream Lexer::lex_script(const fs::path &script_path) {
  ifstream in(script_path);
  if (!in.is_open()) {
    throw "file open error";
  }
  string line;
  bool line_anotation = false;
  string s;
  int state = 0;
  size_t line_num = 0, number = 0, cur_number = 0;
  while (getline(in, line)) {
    for (auto p = line.begin(); p != line.end(); p++) {
      if(_comment_line) continue;
      _ch = *p;
      _it = _operator_table->find(string(1, _ch));
      _is_alpha = isalpha(_ch);
      _is_digit = isdigit(_ch);
      _is_operator = _it != _operator_table->end();

      cout << _ch;

      while(lex_char(state, s));
      number++;
    }
    _comment_line = false;
    _line_num++;
  }
  return _stream;
}

bool Lexer::lex_char (int &state, string &s) {
  switch(state) {
    case 0:
      if(_is_alpha) {
        s.push_back(_ch);
        _cur_number = _number;
        state = 1;
      } else if (_is_digit) {

      } else if (_is_operator) {
        if(_ch == '=') state = 2;
        else {
          _stream.push_back(Token((*_it).second, _line_num, _number));
        }
      } else if (_ch == '#') {
        _comment_line = true;
      } else if (_ch == ' ' || _ch == '\t') {
        
      }
      break;
    case 1:
      if (_is_alpha || _is_digit || _ch == '_') {
        s.push_back(_ch);
      } else {
        lookup_table::iterator k_it =  _keyword_table->find(s);
        if(k_it != _keyword_table->end()) {
          reserved_token token = (*k_it).second;
          _stream.push_back(Token(token, _line_num, _cur_number));
        } else {
          identifier id{_value: s};
          _stream.push_back(Token(id, _line_num, _cur_number));
        }
        s.clear();
        state = 0;
        return 1;
      }
      break;
    case 2:
      state = 0;
      if(_ch == '=') {
        _stream.push_back(Token(reserved_token::EQUAL, _line_num, _cur_number));
      } else {
        _stream.push_back(Token(reserved_token::ASSIGN, _line_num, _cur_number));
        return 1;
      }
      break;
  }
  return 0;
}

unique_ptr<Lexer::lookup_table> LookupTableFactory::get_operator_table() {
  unique_ptr<Lexer::lookup_table> p =
      make_unique<Lexer::lookup_table>(Lexer::lookup_table{
          {"=", reserved_token::ASSIGN},
          {"==", reserved_token::EQUAL},
          {"/", reserved_token::DIV},
          {"*", reserved_token::MUL},
          {"-", reserved_token::SUB},
          {"+", reserved_token::ADD},
          {",", reserved_token::COMMA},
      });

  return p;
}

unique_ptr<Lexer::lookup_table> LookupTableFactory::get_keyword_table() {
  unique_ptr<Lexer::lookup_table> p =
      make_unique<Lexer::lookup_table>(Lexer::lookup_table{
          {"if", reserved_token::IF},
          {"fi", reserved_token::FI},
          {"echo", reserved_token::ECHO},
          {"elif", reserved_token::ELIF},
          {"else", reserved_token::ELSE},
          {"done", reserved_token::DONE},
          {"do", reserved_token::DO},
          {"then", reserved_token::THEN},
          {"call", reserved_token::CALL},
          {"true", reserved_token::TRUE},
          {"false", reserved_token::FALSE},
      });

  return p;
}
