/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 17:45:25
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-19 20:40:22
 */

#include "mainserver.hpp"

#include <iostream>
#include <boost/log/trivial.hpp>
#include "../cpp-httplib/httplib.h"
#include "../lib/jsonxx/jsonxx.h"

using namespace jsonxx;

MainServer::MainServer(statments_table table): _table(table) {}

httplib::Client MainServer::cli = httplib::Client("http://127.0.0.1:5700");

void MainServer::run() {
  httplib::Server server;

  BOOST_LOG_TRIVIAL(info) << "run server in http://127.0.0.1:5700";

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

  server.Get("/stop", [&](const httplib::Request &rep, httplib::Response &res) {
    BOOST_LOG_TRIVIAL(info) << "get a stop message";
    cli.Get("/stop");
    server.stop();
  });

  server.listen("127.0.0.1", 5701);
}
void MainServer::create_qq_thread(qq_id id) {
  auto queue = new QMessageQueue();
  _mqueue[id] = queue;

  Runtime* runtime = new Runtime(&_table);
  auto thread = new Parallel(queue, id, &_table, runtime);

  thread_pool.insert(std::make_pair(id, thread));
}
