/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-30 16:00:11
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 19:41:58
 */
#include "token.hpp"

#include <optional>
#include <string>
#include <variant>

bool Token::is_string() { return _value.index() == 0; }
bool Token::is_reserved_token() { return _value.index() == 1; }
bool Token::is_identifier() { return _value.index() == 2; }
bool Token::is_number() { return _value.index() == 3; }
string Token::get_string() const { return get<string>(_value); }
identifier Token::get_id() const { return get<identifier>(_value); }
reserved_token Token::get_token() const { return get<reserved_token>(_value); }
int Token::get_number() const { return get<int>(_value); }
