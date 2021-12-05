/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 17:45:25
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 13:24:55
 */

#include "mainserver.hpp"

#include <iostream>

#include "../cpp-httplib/httplib.h"
#include "../lib/jsonxx/jsonxx.h"

using namespace jsonxx;

MainServer::MainServer() {}

httplib::Client MainServer::cli = httplib::Client("http://127.0.0.1:5700");

void MainServer::run() {
  httplib::Server server;

  std::cout << "run!";

  server.Post("/", [&](const httplib::Request &rep, httplib::Response &res) {
    Object o, user;
    o.parse(rep.body);
    auto post_type = o.get<String>("post_type");
    if (post_type != "message") {
      return;
    }
    auto message_type = o.get<String>("message_type");
    if (message_type != "private") {
      return;
    }
    user = o.get<Object>("sender");
    auto user_id = static_cast<qq_id>(user.get<Number>("user_id"));
    auto message = o.get<String>("message");

    std::lock_guard<std::mutex> l(thread_pool_mutex);
    if (thread_pool.find(user_id) == thread_pool.end()) {
      create_qq_thread(user_id);
    }

    QMessageQueue *queue = _mqueue[user_id];
    queue->push(QMessage(message));
  });

  server.listen("127.0.0.1", 5701);
}
void MainServer::create_qq_thread(qq_id id) {
  auto queue = new QMessageQueue();
  _mqueue[id] = queue;

  auto thread = new Parallel(queue, id);

  thread_pool.insert(std::make_pair(id, thread));
}
