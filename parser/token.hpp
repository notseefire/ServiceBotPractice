/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 18:44:06
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-11 15:58:22
 */
#pragma once

#include <optional>
#include <string>
#include <variant>

using namespace std;

enum struct reserved_token {
  ECHO = 0,
  INPUT,
  CALL,
  BREAK,
  PROCEDURE,
  EXIT,
  SET,

  ASSIGN,
  ADD,
  COLON,
};

struct identifier {
  string _value;
};

class Token {
 private:
  using token_value = variant<string, reserved_token, identifier>;

  token_value _value;
  size_t _line;
  size_t _number;

 public:
  Token(token_value token, size_t line, size_t number)
      : _value(move(token)), _line(line), _number(number) {}

  bool is_string();
  bool is_reserved_token();
  bool is_identifier();
  string get_string() const;
  identifier get_id() const;
  reserved_token get_token() const;
};