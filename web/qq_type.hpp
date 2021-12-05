/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 17:58:04
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 09:51:59
 */

#pragma once

#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>

using qq_id = long long int;
using qq_pid = short;

class QMessage
{
private:
    std::string _inner;
public:
    QMessage(std::string& inner);
    std::string get_inner();
};

class QMessageQueue
{
public:
    QMessageQueue(/* args */);
    std::unique_lock<std::mutex> get_lock();
    std::unique_lock<std::mutex> wait_data();
    void push(QMessage msg);
    QMessage pop();
private:
    std::mutex _mutex;
    std::condition_variable _condition;
    std::queue<QMessage> _container;
};