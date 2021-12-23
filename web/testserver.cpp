/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-19 12:02:54
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-23 13:15:45
 */

#include "testserver.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "../lib/jsonxx/jsonxx.h"
#include "qq_type.hpp"

using namespace jsonxx;

httplib::Client TestServer::cli = httplib::Client("http://127.0.0.1:5701");

TestServer::TestServer(fs::path path, logging::trivial::severity_level level)
    : _server_thread(&TestServer::run_server, this) {
      logging::core::get()->set_filter(logging::trivial::severity >= level);

      if (!fs::is_directory(path)) {
        throw "Path is not direcotry";
      }
      if (!fs::exists(path)) {
        throw "Directory path not exist";
      }
      std::stringstream sstream;
      for (const auto &entry : fs::directory_iterator(path)) {
        message_queue queue;
        fs::path script_path = entry.path();
        std::ifstream in(script_path);
        std::string message;
        while (in >> message) {
          BOOST_LOG_TRIVIAL(trace)
              << boost::format("add message %1%") % message;
          queue.push_back(std::move(message));
        }
        _mqueue.push_back(std::move(queue));
      }
    }

    void TestServer::run_server() {
      httplib::Server server;
      server.Post("/send_msg",
                  [&](const httplib::Request &rep, httplib::Response &res) {
                    qq_id id = stoi(rep.get_param_value("user_id"));
                    std::string msg = rep.get_param_value("message");

                    BOOST_LOG_TRIVIAL(info)
                        << boost::format("message %1% to %2%") % msg % id;
                  });

      server.Get("/stop", [&](const httplib::Request &rep,
                              httplib::Response &res) { server.stop(); });

      server.listen("127.0.0.1", 5700);
    }

    void TestServer::join() { _server_thread.join(); }

    void TestServer::run(int thread_num) {
      std::vector<std::thread> pool;
      for (int i = 1; i <= thread_num; i++) {
        pool.push_back(std::thread([&, i]() {
          unsigned int seed = i;
          int num = rand_r(&seed) % _mqueue.size();
          qq_id id = rand_r(&seed);
          for (auto index : _mqueue[num]) {
            Object o;
            Object user;
            o << "post_type"
              << "message";
            o << "message_type"
              << "private";
            o << "message" << index;
            user << "user_id" << id;
            o << "sender" << user;

            BOOST_LOG_TRIVIAL(info) << id << "send " << index;
            cli.Post("/", o.json(), "application/json");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
          }
        }));
      }

      for (int i = 0; i < pool.size(); i++) {
        pool[i].join();
      }
      std::this_thread::sleep_for(std::chrono::microseconds(300));
      BOOST_LOG_TRIVIAL(info)
          << "sending all pending message, ready to send stop";
      cli.Get("/stop");
    }
