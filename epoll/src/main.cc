/*
 * Created on Sun Oct 28 2018
 * Author : tianrun
 * Copyright (c) 2018 Senscape
 */
#include "network/little_server.h"
int32_t main(int32_t argc, char** argv) {
  LittleServer little_server;
  if (0 != little_server.LoopForever()) {
    fprintf(stderr, "main:little_server.LoopForever() error\n");
    return -1;
  }
  return 0;
}
