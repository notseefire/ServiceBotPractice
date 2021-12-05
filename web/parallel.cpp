/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 20:14:40
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 11:01:21
 */
#include <mutex>

#include "mainserver.hpp"
#include "parallel.hpp"
#include "../cpp-httplib/httplib.h"


Parallel::Parallel(QMessageQueue* queue, qq_id id) : _queue(queue),
    _id(id), _thread(&Parallel::run, this) {
    _thread.detach();
}

QMessage Parallel::wait_data() {
    auto l = _queue->wait_data();
    return _queue->pop();
}

void Parallel::quit() {
    std::cerr << "quit " << _id << std::endl;
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
  std::cerr << "Output private msg" << std::endl;
}

void Parallel::run() {
    std::cerr << "running" << std::endl;
    send_private_msg("猫猫");
    quit();
}