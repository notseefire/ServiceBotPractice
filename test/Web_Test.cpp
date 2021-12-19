/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 21:06:27
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-19 21:21:21
 */

#include <thread>
#include "../parser/script.hpp"
#include "../parser/parser.hpp"
#include "../parser/lexer.hpp"
#include "../web/mainserver.hpp"
#include "../web/testserver.hpp"
#include "gtest/gtest.h"


TEST(Web, TestServer) {
  fs::path path("/home/cyks/Documents/2021Fall/Homework/Program/assginment/example_scripts");
  auto p = std::make_shared<ScriptManager>(ScriptManager(path));
  LookupTableFactory factory;
  auto keywords = factory.get_keyword_table();
  auto operators = factory.get_operator_table();
  Lexer l(keywords, operators);
  auto token_streams = l.lex(*p);
  Parser parser;
  auto token_table = parser.parser(token_streams);

  TestServer test_server("/home/cyks/Documents/2021Fall/Homework/Program/assginment/test_message");
  std::thread server([token_table]() {
    MainServer server(token_table);
    server.run();
  });
  test_server.run(1);
  server.join();
  test_server.join();
}