/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 12:02:47
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-19 15:38:43
 */

#pragma once
#include <experimental/filesystem>

#include "../cpp-httplib/httplib.h"

namespace fs = std::experimental::filesystem;

class TestServer {
 public:
  static httplib::Client cli;
  TestServer(fs::path path);
  void run(int thread_num);
  void join();

 private:
  void run_server();
  using message_queue = std::vector<std::string>;
  std::thread _server_thread;
  std::vector<message_queue> _mqueue;

};