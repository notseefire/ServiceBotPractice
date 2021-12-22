/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 18:44:06
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-22 17:17:01
 */
#pragma once

#include <optional>
#include <string>
#include <utility>
#include <variant>

using namespace std;

/**
 * @brief keyword and operator appear in script. The type help
 * parser divide code segment and generate AST.
 *
 */
enum struct reserved_token {
  ECHO = 0,
  INPUT,
  CALL,
  BREAK,
  PROCEDURE,
  EXIT,
  SET,
  CHANGE,
  WAIT,
  OTHER,
  LOOPBEGIN,
  LOOPEND,

  ASSIGN,
  ADD,
  COLON,
};

/**
 * @brief identifer type describe variable concept in script, just capsule
 * string inner to tell from normal std::string
 *
 */
struct identifier {
  string _value;
};

/**
 * @brief a type describe Token, its inner is token_value, which consist of
 * modern C++ variant template with `string`, `reserved_token`, `identifer`.
 *
 * And contains information of position, help program detect the error.
 *
 */
class Token {
 private:
  using token_value = variant<string, reserved_token, identifier, int>;

  token_value _value;

 public:
  size_t _line;
  size_t _number;
  /**
   * @brief Construct a new Token object with token kind and position
   *
   * @param token the confirmed token type will be consturcted
   * @param line line in shell text
   * @param number column in shell text
   */
  Token(token_value token, size_t line, size_t number)
      : _value(move(token)), _line(line), _number(number) {}

  /**
   * @brief check whether if the token type is string
   *
   * @return true if token value is string
   */
  bool is_string();

  /**
   * @brief check whether if the token type is reserved_token
   *
   * @return true if token value is reserved_token
   */
  bool is_reserved_token();

  /**
   * @brief check whether if the token type is identifier
   *
   * @return true if token value is identifier
   */
  bool is_identifier();

  /**
   * @brief check whether if the token type is number
   * 
   * @return true if token value is number
   */
  bool is_number();

  /**
   * @brief Get the string object
   *
   * @return string
   */
  string get_string() const;

  /**
   * @brief Get the identifier object
   *
   * @return identifier
   */
  identifier get_id() const;

  /**
   * @brief Get the reserved_token object
   *
   * @return reserved_token
   */
  reserved_token get_token() const;

  /**
   * @brief Get the number object
   *
   * @return number
   */
  int get_number() const;
};
