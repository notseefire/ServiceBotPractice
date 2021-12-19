/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-01 22:41:56
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 01:14:15
 */

#pragma once

#include <thread>

#include "../cpp-httplib/httplib.h"
#include "qq_type.hpp"
#include "./runtime.hpp"

class AstStatement;
class Runtime;

using statments = std::vector<AstStatement*>;
using statments_table = std::map<std::string, statments>;

/**
 * @brief  Parallel held a thread class as a attribute and construct last in 
 * itself construction. I make it capsule a thread as you can easily create a
 * thread receiving user's message.
 * 
 * Note that Parallel contains a `Runtime` attibute, the script provided run in `Runtime`,
 * and Parallel's other function like `send_private_msg` and `wait_data` will be called in `Runtime`
 * 
 */
class Parallel {
 public:
  /**
   * @brief Construct a new Parallel object, note runtime is initialied outsize this construction,
   * the main reason is design issue and  can't initialize a `Runtime` in Parallel as they are interdependent.
   * 
   * @param queue pointer to QMessage queue
   * @param id user's qq id
   * @param p_table pointer to all block
   * @param runtime contains context when runing scripts
   */
  Parallel(QMessageQueue* queue, qq_id id, statments_table* p_table, Runtime* runtime);


  /**
   * @brief run `Runtime`
   * 
   */
  void run();

  /**
   * @brief quit thread, this function will erase the thread now from thread pool.
   * which resulting thread close. note it request `thread_pool_mutex`
   * 
   */
  void quit();

  /**
   * @brief  wait message comming or get message having been contained in queue
   * 
   * @return QMessage 
   */
  QMessage wait_data();

  /**
   * @brief send the message to the spefic user in thread, it send http request by
   * client in `MainServer`
   * 
   * @param msg  string message
   */
  void send_private_msg(std::string msg);

 private:
  QMessageQueue* _queue;
  qq_id _id;
  std::thread _thread;
  Runtime* _runtime;
};