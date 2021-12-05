/*
 * @Descripttion: 
 * @version: 1.0.0
 * @Author: CYKS
 * @Date: 2021-11-29 10:30:36
 * @LastEditors: CYKS
 * @LastEditTime: 2021-12-05 09:45:41
 */
#include <iostream>

#include "./web/mainserver.hpp"

int main(int argc, char** argv) {
  MainServer server;

  server.run();
}