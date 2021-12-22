/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 20:14:40
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-22 16:00:08
 */
#include <mutex>
#include <string>
#include <optional>
#include <boost/log/trivial.hpp>

#include "parallel.hpp"
#include "../cpp-httplib/httplib.h"
#include "mainserver.hpp"

Parallel::Parallel(QMessageQueue* queue, qq_id id, statments_table* p_table,
                   Runtime* runtime)
    : _queue(queue), _id(id), _runtime(runtime), _thread(&Parallel::run, this) {
  _thread.detach();
}

std::optional<QMessage> Parallel::wait_data(int micro_seconds) { return _queue->pop(micro_seconds); }

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
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void Parallel::run() {
  while (!_runtime->end) {
    _runtime->step(this);
  }
  quit();
}

Parallel::~Parallel() {
  delete _queue;
  delete _runtime;
}