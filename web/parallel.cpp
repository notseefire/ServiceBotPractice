/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 20:14:40
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-19 21:14:05
 */
#include "parallel.hpp"

#include <boost/log/trivial.hpp>
#include <mutex>

#include "../cpp-httplib/httplib.h"
#include "mainserver.hpp"

Parallel::Parallel(QMessageQueue* queue, qq_id id, statments_table* p_table, Runtime* runtime)
    : _queue(queue), _id(id), _runtime(runtime), _thread(&Parallel::run, this) {
  _thread.detach();
}

QMessage Parallel::wait_data() { return _queue->pop(); }

void Parallel::quit() {
  std::lock_guard<std::mutex> l(MainServer::thread_pool_mutex);
  delete _queue;
  MainServer::thread_pool.erase(_id);
}

void Parallel::send_private_msg(std::string msg) {
  httplib::Params params;
  params.emplace("message_type", "private");
  params.emplace("user_id", std::to_string(_id));
  params.emplace("message", msg);
  std::lock_guard<std::mutex> l(MainServer::cli_mutex);
  try {
    MainServer::cli.Post("/send_msg", params);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void Parallel::run() {
  while (!_runtime->end) {
    _runtime->step(this);
  }
  quit();
}