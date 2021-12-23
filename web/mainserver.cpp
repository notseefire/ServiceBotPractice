/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 17:45:25
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-23 13:12:49
 */


#include <iostream>
#include <utility>
#include <future>
#include <fstream>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include "mainserver.hpp"
#include "../cpp-httplib/httplib.h"
#include "../lib/jsonxx/jsonxx.h"

using namespace jsonxx;
namespace logging = boost::log;

MainServer::MainServer(statments_table table, fs::path db_path): _table(table), _db_path(db_path){
  if(!fs::is_directory(db_path)) {
    throw "path should be directory";
  }
  logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
}

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

  std::promise<void> signal_exit;

  server.Get("/stop", [&](const httplib::Request &rep, httplib::Response &res) {
    BOOST_LOG_TRIVIAL(info) << "get a stop message";
    cli.Get("/stop");
    server.stop();
  });

  std::thread free_thread([&](std::future<void> future){
    while(true) {
      {
        std::lock_guard<std::mutex> l_pool(thread_pool_mutex);
        if(thread_pool.empty() && future.wait_for(std::chrono::milliseconds(1)) != std::future_status::timeout) {
          break;
        }
      }
      std::this_thread::sleep_for(std::chrono::microseconds(30));
      std::lock_guard<std::mutex> l(free_mutex);
      while(!free_queue.empty()) {
        auto element = free_queue.front();
        BOOST_LOG_TRIVIAL(info) << "delete thread " << element->_id ;
        
        fs::path json_path = _db_path / fs::path(std::to_string(element->_id) + ".json");
        ofstream out(json_path.string());

        out << element->_db.json();
        delete element;
        free_queue.pop();
      }
    }
  }, std::move(signal_exit.get_future()));

  server.listen("127.0.0.1", 5701);
  signal_exit.set_value();
  free_thread.join();
}
void MainServer::create_qq_thread(qq_id id) {
  auto queue = new QMessageQueue();
  _mqueue[id] = queue;

  fs::path json_path = _db_path / fs::path(std::to_string(id) + ".json");
  Runtime* runtime = new Runtime(&_table);
  jsonxx::Object o;

  if(fs::exists(json_path)) {
    ifstream in(json_path.string(), ifstream::in);

    std::string json_content((std::istreambuf_iterator<char>(in)),
                            std::istreambuf_iterator<char>());

    o.parse(json_content);
    in.close();
  }
  
  auto thread = new Parallel(queue, id, &_table, runtime, o);

  thread_pool.insert(std::make_pair(id, thread));
}
