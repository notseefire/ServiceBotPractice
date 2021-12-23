/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 17:45:19
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-23 10:49:48
 */

#pragma once

#include <map>
#include <mutex>
#include <queue>
#include <set>

#include "../cpp-httplib/httplib.h"
#include "parallel.hpp"
#include "qq_type.hpp"
#include "statment.hpp"

/**
 * @brief the server framework held thread_pool. it manages resource thread need
 * but shared among each other. such as message queue, thread pool. So note
 * thread-safety when using `MainServer`
 *
 */
class MainServer {
 public:
  inline static std::map<qq_id, Parallel*> thread_pool;
  inline static std::mutex free_mutex;
  inline static std::queue<Parallel *> free_queue;
  inline static std::mutex thread_pool_mutex;
  inline static std::mutex cli_mutex;
  static httplib::Client cli;
  /**
   * @brief Construct a new Main Server object
   *
   * @param _table contains all block name and block body from ast.
   */
  explicit MainServer(statments_table _table);

  /**
   * @brief server start to listen on port
   *
   */
  void run();

  /**
   * @brief Create a qq thread object into thread pool, this operator will
   * request mutex `thread_pool_mutex` as it wanna append into `thread_pool`.
   * It should be avoided that *deadlock* by `thread_pool_mutex`
   *
   * @param id id of qq user
   */
  void create_qq_thread(qq_id id);

 private:
  std::map<qq_id, QMessageQueue*> _mqueue;
  statments_table _table;
};
