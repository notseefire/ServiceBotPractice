/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-12-01 22:41:56
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 10:52:49
 */

#pragma once

#include<thread>


#include "../cpp-httplib/httplib.h"
#include "qq_type.hpp"

class Parallel {
 public:
    Parallel(QMessageQueue* queue, qq_id id);

    void run();
    void quit();
    QMessage wait_data();
    void send_private_msg(std::string msg);
    
 private:
    QMessageQueue* _queue;
    qq_id _id;
    std::thread _thread;
};