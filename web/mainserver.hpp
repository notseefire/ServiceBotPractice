/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 17:45:19
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 10:51:05
 */

#pragma once

#include <map>
#include <mutex>
#include <queue>
#include <set>

#include "../cpp-httplib/httplib.h"
#include "parallel.hpp"
#include "qq_type.hpp"

class MainServer{
 public:
  inline static std::map<qq_id, Parallel*> thread_pool;
  inline static std::mutex thread_pool_mutex;
  inline static std::mutex cli_mutex;
  static httplib::Client cli;
  MainServer();
  void run();
  void create_qq_thread(qq_id id);
 private:
  std::map<qq_id, QMessageQueue*> _mqueue;
};