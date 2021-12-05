/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 18:36:20
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 13:25:56
 */

#include "qq_type.hpp"

#include <mutex>

QMessage::QMessage(std::string& inner) : _inner(inner) {}

std::string QMessage::get_inner() { return _inner; }

QMessageQueue::QMessageQueue() { _ready = false; }

std::unique_lock<std::mutex> QMessageQueue::get_lock() {
  std::unique_lock<std::mutex> head_lock(_mutex);
  return std::move(head_lock);
}

std::unique_lock<std::mutex> QMessageQueue::wait_data() {
  std::unique_lock<std::mutex> head_lock(_mutex);
  _condition.wait(head_lock, [&] { return _ready; });
  return std::move(head_lock);
}

void QMessageQueue::push(QMessage msg) {
  auto l = get_lock();
  _container.push(msg);
  if (_ready == false) {
    _ready = true;
    l.unlock();
    _condition.notify_one();
  }
}

QMessage QMessageQueue::pop() {
  auto l = wait_data();
  QMessage msg = _container.front();
  _container.pop();
  if (_container.empty()) {
    _ready = false;
  }
  return msg;
}
