/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 10:30:36
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 15:21:17
 */
#include <iostream>

#include "./web/mainserver.hpp"

httplib::Client temp("http://127.0.0.1:5700");

int main(int argc, char** argv) {
  httplib::Params params;
  params.emplace("message_type", "private");
  params.emplace("user_id", std::to_string(370797515ll));
  params.emplace("message", "服务器已启动");
  auto res = temp.Post("/send_msg", params);
  MainServer server;
  server.run();
}