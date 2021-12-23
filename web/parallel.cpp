/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 20:14:40
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-23 12:11:23
 */
#include "parallel.hpp"

#include <boost/log/trivial.hpp>
#include <mutex>
#include <optional>
#include <string>

#include "../cpp-httplib/httplib.h"
#include "mainserver.hpp"

Parallel::Parallel(QMessageQueue* queue, qq_id id, statments_table* p_table,
                   Runtime* runtime, jsonxx::Object obj)
    : _p_table(p_table),
      _queue(queue),
      _id(id),
      _runtime(runtime),
      _db(obj),
      _thread(&Parallel::run, this) {
  _thread.detach();
}

std::optional<QMessage> Parallel::wait_data(int micro_seconds) {
  return _queue->pop(micro_seconds);
}

void Parallel::quit() {
  {
    std::lock_guard<std::mutex> l(MainServer::thread_pool_mutex);
    MainServer::thread_pool.erase(_id);
  }
  std::lock_guard<std::mutex> l2(MainServer::free_mutex);
  MainServer::free_queue.push(this);
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
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void Parallel::run() {
  _runtime = new Runtime(_p_table);
  while (!_runtime->end) {
    _runtime->step(this);
  }
  quit();
}

Parallel::~Parallel() {
  delete _queue;
  delete _runtime;
}