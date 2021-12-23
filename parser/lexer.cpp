/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 21:07:50
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-23 10:47:14
 */

#include "lexer.hpp"

#include <fstream>
#include <string>
#include <utility>

#include "token.hpp"

using namespace std;

Lexer::Lexer(unique_ptr<lookup_table> &keyword_table,
             unique_ptr<lookup_table> &operator_table,
             logging::trivial::severity_level level)
    : _keyword_table(move(keyword_table)),
      _operator_table(move(operator_table)) {
  logging::core::get()->set_filter(logging::trivial::severity >= level);
}

map<string, Lexer::token_stream> Lexer::lex(ScriptManager &manager) {
  for (auto t = manager.begin(); t != manager.end(); t++) {
    Lexer::token_stream stream = lex_script((*t).second);
    _token_table[(*t).first] = stream;
  }
  if(!_error_queue.empty()) {
    for(auto index: _error_queue) {
      BOOST_LOG_TRIVIAL(error) << index;
    }
    throw "lexer error";
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
  _line_num = 1, _number = 1, _cur_number = 1;
  while (getline(in, line)) {
    line.push_back('\n');
    for (auto p = line.begin(); p != line.end(); p++) {
      if (_comment_line) continue;
      _ch = *p;
      _it = _operator_table->find(string(1, _ch));
      _is_alpha = (_ch >= 'a' && _ch <= 'z') || (_ch >= 'A' && _ch <= 'Z');
      _is_digit = isdigit(_ch);
      _is_operator = _it != _operator_table->end();
      try {
        while (lex_char(state, s)) continue;
      } catch (const char *str) {
        state = 0;
        _error_queue.push_back(str);
      }
      _number++;
    }
    _comment_line = false;
    _line_num++;
    _number = 0;
    _cur_number = 0;
    /*
    for (auto p = _stream.begin(); p != _stream.end(); p++) {
      if (p->is_string()) {
        cout << p->get_string() << " ";
      }
      if (p->is_identifier()) {
        cout << p->get_id()._value << " ";
      }
      if (p->is_reserved_token()) {
        cout << (int)p->get_token() << " ";
      }
    }
    std::cout << std::endl;
    */
  }

  return _stream;
}
char Lexer::transfer() {
  if (_ch == 'n') {
    return '\n';
  } else {
    return _ch;
  }
}

bool Lexer::lex_char(int &state, string &s) {
  switch (state) {
    case 0:
      if (_is_alpha || _ch == '_') {
        s.push_back(_ch);
        _cur_number = _number;
        state = 1;
      } else if (_is_digit) {
        s.push_back(_ch);
        _cur_number = _number;
        state = 7;
      } else if (_is_operator) {
        _stream.push_back(Token((*_it).second, _line_num, _number));
      } else if (_ch == '#') {
        _comment_line = true;
      } else if (_ch == '"') {
        state = 3;
      } else if (_ch == '\'') {
        state = 5;
      } else if (_ch == ' ' || _ch == '\t' || _ch == '\n') {
      } else {
      }
      break;
    case 1:
      if (_is_alpha || _is_digit || _ch == '_') {
        s.push_back(_ch);
      } else {
        lookup_table::iterator k_it = _keyword_table->find(s);
        if (k_it != _keyword_table->end()) {
          reserved_token token = (*k_it).second;
          _stream.push_back(Token(token, _line_num, _cur_number));
        } else {
          identifier id{_value : s};
          _stream.push_back(Token(id, _line_num, _cur_number));
        }
        s.clear();
        state = 0;
        return 1;
      }
      break;
    case 3:
      if (_ch == '\\') {
        state = 4;
      } else if (_ch == '"') {
        _stream.push_back(Token(s, _line_num, _cur_number));
        s.clear();
        state = 0;
      } else {
        s.push_back(_ch);
      }
      break;
    case 4:
      s.push_back(transfer());
      state = 3;
      break;
    case 5:
      if (_ch == '\\') {
        state = 6;
      } else if (_ch == '\'') {
        _stream.push_back(Token(s, _line_num, _cur_number));
        s.clear();
        state = 0;
      } else {
        s.push_back(_ch);
      }
      break;
    case 6:
      s.push_back(transfer());
      state = 6;
      break;
    case 7:
      if(_is_digit) {
        s.push_back(_ch);
      } else {
        _stream.push_back(Token(stoi(s), _line_num, _cur_number));
        s.clear();
        state = 0;
        return 1;
      }
      break;
  }
  return 0;
}

unique_ptr<Lexer::lookup_table> LookupTableFactory::get_operator_table() {
  unique_ptr<Lexer::lookup_table> p =
      make_unique<Lexer::lookup_table>(Lexer::lookup_table{
          {"+", reserved_token::OP_ADD},
          {"=", reserved_token::ASSIGN},
          {":", reserved_token::COLON},
      });

  return p;
}

unique_ptr<Lexer::lookup_table> LookupTableFactory::get_keyword_table() {
  unique_ptr<Lexer::lookup_table> p =
      make_unique<Lexer::lookup_table>(Lexer::lookup_table{
          {"echo", reserved_token::ECHO},
          {"input", reserved_token::INPUT},
          {"call", reserved_token::CALL},
          {"break", reserved_token::BREAK},
          {"proc", reserved_token::PROCEDURE},
          {"exit", reserved_token::EXIT},
          {"set", reserved_token::SET},
          {"assign", reserved_token::CHANGE},
          {"wait", reserved_token::WAIT},
          {"other", reserved_token::OTHER},
          {"loop", reserved_token::LOOPBEGIN},
          {"endloop", reserved_token::LOOPEND},
          {"store"},
          {"load"},
      });

  return p;
}
