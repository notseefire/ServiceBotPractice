/*
 * @Descripttion:
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 17:58:04
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-20 13:54:47
 */

#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <optional>

using qq_id = int64_t;
using qq_pid = int16_t;

/**
 * @brief a capsule for message from qq, but i don't have idea about how to
 * use it. now it just have an inner and visit it.
 * 
 */
class QMessage {
 private:
  std::string _inner;

 public:
  explicit QMessage(std::string& inner);
  std::string get_inner();
};

/**
 * @brief QMessageQueue is a real thread-safe queue, every QMessageQueue has its own
 * mutex for queue container. It's designed mainly in condition variable.
 * 
 */
class QMessageQueue {
 public:
  QMessageQueue();
  /**
   * @brief push a QMessage into queue, if queue is empty, the _ready will be set to true
   * 
   * @param msg 
   */
  void push(QMessage msg);

  /**
   * @brief pop and return QMessage from queue front, if queue is empty, the _ready will be set to false.
   * `pop` use `wait_data` as a condtion variable style.
   * 
   * @return QMessage 
   */
  std::optional<QMessage> pop(int micro_seconds);

  std::unique_lock<std::mutex> get_lock();

 private:
  std::unique_lock<std::mutex> wait_data(int micro_seconds);

  /**
   * @brief  mutex for `_containner`
   * 
   */
  std::mutex _mutex;

  /**
   * @brief condition variabel for `_ready`
   * 
   */
  std::queue<QMessage> _container;
  bool _ready;
};
