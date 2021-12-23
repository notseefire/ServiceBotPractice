/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 21:06:27
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-23 12:15:13
 */

#include <thread>

#include "../parser/lexer.hpp"
#include "../parser/parser.hpp"
#include "../parser/script.hpp"
#include "../web/mainserver.hpp"
#include "../web/testserver.hpp"
#include "gtest/gtest.h"

TEST(Web, Regular) {
  fs::path path(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/"
      "example_scripts/regular");

  fs::path db_path(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/db/regular");
  auto p = std::make_shared<ScriptManager>(ScriptManager(path));
  LookupTableFactory factory;
  auto keywords = factory.get_keyword_table();
  auto operators = factory.get_operator_table();
  Lexer l(keywords, operators, logging::trivial::info);
  auto token_streams = l.lex(*p);
  Parser parser;
  auto token_table = parser.parser(token_streams);

  TestServer test_server(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/test_message/"
      "regular",
      logging::trivial::trace);
  std::thread server([token_table, db_path]() {
    MainServer server(token_table, db_path);
    server.run();
  });
  test_server.run(1);
  server.join();
  test_server.join();
}

TEST(Web, Echo) {
  fs::path path(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/"
      "example_scripts/echo");
  fs::path db_path(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/db/echo");
  auto p = std::make_shared<ScriptManager>(ScriptManager(path));
  LookupTableFactory factory;
  auto keywords = factory.get_keyword_table();
  auto operators = factory.get_operator_table();
  Lexer l(keywords, operators, logging::trivial::info);
  auto token_streams = l.lex(*p);
  Parser parser;
  auto token_table = parser.parser(token_streams);

  TestServer test_server(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/test_message/"
      "echo",
      logging::trivial::trace);
  std::thread server([token_table, db_path]() {
    MainServer server(token_table, db_path);
    server.run();
  });
  test_server.run(1);
  server.join();
  test_server.join();
}

TEST(Web, Wait) {
  fs::path path(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/"
      "example_scripts/wait_timeout");
  fs::path db_path(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/db/wait_timeout");
  auto p = std::make_shared<ScriptManager>(ScriptManager(path));
  LookupTableFactory factory;
  auto keywords = factory.get_keyword_table();
  auto operators = factory.get_operator_table();
  Lexer l(keywords, operators, logging::trivial::info);
  auto token_streams = l.lex(*p);
  Parser parser;
  auto token_table = parser.parser(token_streams);

  TestServer test_server(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/test_message/"
      "wait_timeout",
      logging::trivial::trace);
  
  std::thread server([token_table, db_path]() {
    MainServer server(token_table, db_path);
    server.run();
  });

  test_server.run(1);
  server.join();
  test_server.join();
}

TEST(Web, Company) {
  fs::path path(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/"
      "example_scripts/company");
  fs::path db_path(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/db/company");
  auto p = std::make_shared<ScriptManager>(ScriptManager(path));
  LookupTableFactory factory;
  auto keywords = factory.get_keyword_table();
  auto operators = factory.get_operator_table();
  Lexer l(keywords, operators, logging::trivial::info);
  auto token_streams = l.lex(*p);
  Parser parser;
  auto token_table = parser.parser(token_streams);

  TestServer test_server(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/test_message/"
      "company",
      logging::trivial::trace);
  
  std::thread server([token_table, db_path]() {
    MainServer server(token_table, db_path);
    server.run();
  });

  test_server.run(1);
  server.join();
  test_server.join();
}