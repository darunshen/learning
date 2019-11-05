/*
 * Created on Sun Sep 30 2018
 * Author : tianrun
 * Copyright (c) 2018 OPEN
 */
#include "src/network/little_server.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define FILE_BUFFER 1000
#define LISTEN_PORT 8899
int32_t LittleServer::CreateListener(const int32_t port, const char* bind_ip) {
  if (bind_ip == NULL) {
    fprintf(stderr, "CreateListener : bind_ip is NULL\n");
    return -1;
  }
  int32_t listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == listen_fd) {
    perror("LoopForever:socket(AF_INET, SOCK_STREAM | SOCK_STREAM, 0)");
    return -1;
  }
  listen_fd_.push_back(listen_fd);
  struct sockaddr_in listen_addr;
  bzero(&listen_addr, sizeof(listen_addr));
  if (0 == inet_aton("127.0.0.1", &listen_addr.sin_addr)) {
    fprintf(stderr,
            "CreateListener:inet_aton(%s, &listen_addr.sin_addr) failed\n",
            bind_ip);
    return -1;
  }
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_port = htons(port);
  if (-1 == bind(listen_fd, reinterpret_cast<sockaddr*>(&listen_addr),
                 sizeof(listen_addr))) {
    perror("LoopForever:bind(listen_fd,&listen_addr,sizeof(listen_addr))");
    return -1;
  }
  if (-1 == listen(listen_fd, 6)) {
    perror("LoopForever:listen(listen_fd,1024)");
    return -1;
  }
  struct epoll_event listen_event;
  listen_event.events = EPOLLIN;
  listen_event.data.fd = listen_fd;
  epoll_ctl(this->epoll_fd_, EPOLL_CTL_ADD, listen_fd, &listen_event);
  return 0;
}
int32_t LittleServer::LoopForever() {
  this->epoll_fd_ = epoll_create(1);
  if (this->epoll_fd_ == -1) {
    perror("LoopForever:epoll_create(1)");
    return -1;
  }
  if (-1 == CreateListener(LISTEN_PORT)) {
    fprintf(stderr, "LoopForever:CreateListener(%d) error\n", LISTEN_PORT);
    return -1;
  }
  while (1) {
    actived_fd_num_ = epoll_wait(this->epoll_fd_, this->events_, 100, -1);
    if (actived_fd_num_ == -1 || this->listen_fd_.size() <= 0) {
      perror(
          "LoopForever:epoll_wait(this->epoll_fd_, this->events_, 100, -1) or "
          "this->listen_fd_.size() <= 0");
      exit(-1);
    }

    for (int i = 0; i < actived_fd_num_; i++) {
      int32_t listen_iterator = 0;
      for (; listen_iterator < this->listen_fd_.size(); listen_iterator++) {
        if (this->events_[i].data.fd == this->listen_fd_.at(listen_iterator)) {
          int32_t accept_fd =
              accept(this->listen_fd_.at(listen_iterator), NULL, NULL);
          if (-1 == accept_fd || -1 == this->SetNonBlocking(accept_fd)) {
            perror(
                "LoopForever:accept(this->listen_fd_.at(listen_iterator), "
                "NULL, NULL) or "
                "this->SetNonBlocking(accept_fd)");
            continue;
          }
          struct epoll_event accept_event;
          accept_event.events = EPOLLIN | EPOLLET;
          accept_event.data.fd = accept_fd;
          epoll_ctl(this->epoll_fd_, EPOLL_CTL_ADD, accept_fd, &accept_event);
          break;
        }
      }
      if (listen_iterator == this->listen_fd_.size()) {
        int32_t read_data, write_data, seek = 0;
        char file_buffer[FILE_BUFFER] = {0};
        while ((read_data =
                    read(events_[i].data.fd, file_buffer, FILE_BUFFER)) > 0) {
          write_data = write(events_[i].data.fd, file_buffer, read_data);
          if (-1 == write_data) {
            perror(
                "LoopForever:write(events_[i].data.fd, file_buffer, "
                "read_data)");
            continue;
          } else if (0 == write_data) {
            fprintf(stderr,
                    "write zero data to socket but the read data is %d\n",
                    read_data);
            continue;
          }
        }
        if (read_data == -1) {
          perror(
              "LoopForever:read(events_[i].data.fd, file_buffer, FILE_BUFFER)");
          continue;
        }
      }
    }
  }
}
int32_t LittleServer::SetNonBlocking(const int32_t fd) {
  if (fd <= 0) {
    fprintf(stderr, "the input error! fd=%d\n", fd);
    return -1;
  }
  int32_t flag = fcntl(fd, F_GETFD);
  if (flag == -1) {
    perror("SetNonBlocking:fcntl(fd,F_GETFD)");
    return -1;
  }
  flag |= O_NONBLOCK;
  if (-1 == (fcntl(fd, F_SETFD, flag))) {
    perror("SetNonBlocking:fcntl(fd,F_SETFD)");
    return -1;
  }
  return 0;
}
