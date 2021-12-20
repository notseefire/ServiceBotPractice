/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 12:02:47
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 15:39:56
 */

#pragma once
#include <experimental/filesystem>
#include <string>
#include <vector>

#include <boost/format.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include "../cpp-httplib/httplib.h"

namespace logging = boost::log;
namespace fs = std::experimental::filesystem;

class TestServer {
 public:
  static httplib::Client cli;
  explicit TestServer(fs::path path, logging::trivial::severity_level level);
  void run(int thread_num);
  void join();

 private:
  void run_server();
  using message_queue = std::vector<std::string>;
  std::thread _server_thread;
  std::vector<message_queue> _mqueue;
};
