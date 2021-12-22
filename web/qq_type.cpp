/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 18:36:20
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 19:54:40
 */

#include <string>
#include <utility>
#include <mutex>
#include <optional>
#include <thread>

#include "qq_type.hpp"


QMessage::QMessage(std::string& inner) : _inner(inner) {}

std::string QMessage::get_inner() { return _inner; }

QMessageQueue::QMessageQueue() { _ready = false; }

std::unique_lock<std::mutex> QMessageQueue::get_lock() {
  std::unique_lock<std::mutex> head_lock(_mutex);
  return std::move(head_lock);
}

std::unique_lock<std::mutex> QMessageQueue::wait_data(int micro_seconds) {
  std::unique_lock<std::mutex> head_lock(_mutex);
  int count = 0;
  while(!_ready && count < micro_seconds) {
    head_lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    count += 100;
    head_lock.lock();
  }
  return std::move(head_lock);
}

void QMessageQueue::push(QMessage msg) {
  auto l = get_lock();
  _container.push(msg);
  if (_ready == false) {
    _ready = true;
    l.unlock();
  }
}

std::optional<QMessage> QMessageQueue::pop(int micro_seconds) {
  auto l = wait_data(micro_seconds);
  if(_ready == false) {
    return std::nullopt;
  }
  QMessage msg = _container.front();
  _container.pop();
  if (_container.empty()) {
    _ready = false;
  }
  return msg;
}
