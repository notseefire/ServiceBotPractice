/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 18:26:51
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-19 11:52:49
 */

#include <stdexcept>
#include <boost/log/trivial.hpp>

#include "parser.hpp"
#include "ast.hpp"

Parser::token_table Parser::parser(map<string, Lexer::token_stream> table) {
  token_table new_table;
  for (auto index = table.begin(); index != table.end(); index++) {
    Lexer::token_stream stream = (*index).second;
    Lexer::token_stream::iterator begin, end, stream_end = stream.end();
    auto cur = stream.begin();
    if(cur == stream_end) {
      BOOST_LOG_TRIVIAL(warning) << (*index).first << " is blank, skip";
      continue;
    }
    BOOST_LOG_TRIVIAL(info) << "parser " << (*index).first;
    string name = find_proc(cur, stream_end);
    begin = cur;
    for (; cur != stream.end(); cur++) {
      if (cur->is_reserved_token() &&
          cur->get_token() == reserved_token::PROCEDURE) {
        end = cur;
        new_table.insert(std::make_pair(name, Block(name, begin, cur)._stmts));
        BOOST_LOG_TRIVIAL(info) << "detect and adding procedure " << name;
        name = find_proc(cur, stream_end);
        begin = cur;
      }
    }
    new_table.insert(std::make_pair(name, Block(name, begin, stream_end)._stmts));
  }
  return std::move(new_table);
}
string Parser::find_proc(Lexer::token_stream::iterator &cur,
                         Lexer::token_stream::iterator &end) {
  if (cur == end || !cur->is_reserved_token()) {
    throw "expect proc defination";
  }

  auto token = cur->get_token();
  if (token != reserved_token::PROCEDURE) {
    throw "expect proc defination";
  }

  cur++;
  if (cur == end || !cur->is_identifier()) {
    throw "expect proc name";
  }

  auto id = cur->get_id();
  cur++;

  if (cur == end || !(cur->is_reserved_token() && cur->get_token() == reserved_token::COLON)) {
    throw "expect ':' after declare a proc";
  }

  cur++;

  return id._value;
}