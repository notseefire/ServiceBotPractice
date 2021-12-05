/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 20:14:40
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 10:09:16
 */
#include <mutex>

#include "mainserver.hpp"
#include "parallel.hpp"
#include "../cpp-httplib/httplib.h"


Parallel::Parallel(QMessageQueue* queue, qq_id id) : _queue(queue),
    _id(id), _cli("http://127.0.0.1:5700"),
    _thread(&Parallel::run, this) {
    _thread.detach();
}

QMessage Parallel::wait_data() {
    auto l = _queue->wait_data();
    return _queue->pop();
}

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
  _cli.Post("/send_msg", params);
}

void Parallel::run() {
    send_private_msg("猫猫");
    quit();
}