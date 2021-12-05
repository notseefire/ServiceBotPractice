/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-04 18:36:20
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 09:53:16
 */

#include <mutex>

#include "qq_type.hpp"

QMessage::QMessage(std::string& inner): _inner(inner) {

}


std::string QMessage::get_inner() {
    return _inner;
}

QMessageQueue::QMessageQueue() {

}

std::unique_lock<std::mutex> QMessageQueue::get_lock() {
    std::unique_lock<std::mutex> head_lock(_mutex);
    return std::move(head_lock);
}

std::unique_lock<std::mutex> QMessageQueue::wait_data() {
    std::unique_lock<std::mutex> head_lock(_mutex);
    _condition.wait(head_lock, [&]{return !_container.empty();});
    return std::move(head_lock);
}

void QMessageQueue::push(QMessage msg) {
    _container.push(msg);
}

QMessage QMessageQueue::pop() {
    QMessage msg = _container.front();
    _container.pop();
    return msg;
}
