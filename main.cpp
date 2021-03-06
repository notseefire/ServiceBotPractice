/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 10:30:36
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-23 12:07:50
 */
#include <iostream>

#include "./parser/lexer.hpp"
#include "./parser/parser.hpp"
#include "./parser/script.hpp"
#include "./web/mainserver.hpp"
#include "./web/testserver.hpp"

int main(int argc, char** argv) {
  fs::path path(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/"
      "example_scripts/course");

  fs::path db_path(
      "/home/cyks/Documents/2021Fall/Homework/Program/assginment/db/course");
  auto p = std::make_shared<ScriptManager>(ScriptManager(path));
  LookupTableFactory factory;
  try {
    auto keywords = factory.get_keyword_table();
    auto operators = factory.get_operator_table();
    Lexer l(keywords, operators, logging::trivial::trace);
    auto token_streams = l.lex(*p);
    Parser parser;
    auto token_table = parser.parser(token_streams);
    for (auto index = token_table.begin(); index != token_table.end();
         index++) {
      std::cout << index->first << std::endl;
    }
    httplib::Client temp("http://127.0.0.1:5700");
    httplib::Params params;
    params.emplace("message_type", "private");
    params.emplace("user_id", std::to_string(370797515ll));
    params.emplace("message", "服务器已启动");
    auto res = temp.Post("/send_msg", params);
    MainServer server(token_table, db_path);
    server.run();
  } catch (const char* msg) {
    BOOST_LOG_TRIVIAL(error) << msg;
  }
}