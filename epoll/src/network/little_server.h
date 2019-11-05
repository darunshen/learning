/*
 * Created on Sun Sep 30 2018
 * Author : tianrun
 * Copyright (c) 2018 OPEN
 */
#ifndef SRC_NETWORK_LITTLE_SERVER_H_
#define SRC_NETWORK_LITTLE_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
class LittleServer {
 public:
  /**
   * @brief
   *
   * @return int32_t 0:success -1:error
   */
  int32_t LoopForever();

 protected:
  /**
   * @brief Set the target 'fd' to Non Blocking mode
   *
   * @param [in] fd
   * @return int32_t 0:success; -1:error
   */
  int32_t SetNonBlocking(const int32_t fd);
  /**
   * @brief Create a Listen Socket and add it to epoll
   *
   */
  int32_t CreateListener(const int32_t port, const char* bind_ip = "127.0.0.1");

 private:
  int32_t epoll_fd_;
  struct epoll_event events_[100];
  vector<int32_t> listen_fd_;
  int32_t actived_fd_num_;
};
#endif  // SRC_NETWORK_LITTLE_SERVER_H_
