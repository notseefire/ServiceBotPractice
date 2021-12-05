/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 17:58:04
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 13:41:01
 */

#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

using qq_id = long long int;
using qq_pid = short;

class QMessage {
 private:
  std::string _inner;

 public:
  QMessage(std::string& inner);
  std::string get_inner();
};

class QMessageQueue {
 public:
  QMessageQueue(/* args */);
  void push(QMessage msg);
  QMessage pop();

 private:
  std::unique_lock<std::mutex> get_lock();
  std::unique_lock<std::mutex> wait_data();

  std::mutex _mutex;
  std::condition_variable _condition;
  std::queue<QMessage> _container;
  bool _ready;
};