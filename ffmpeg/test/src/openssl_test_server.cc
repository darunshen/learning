/*
 * Created on Fri Nov 01 2019
 * Copyright (c) 2019
 */
#include <pthread.h>
#include "openssl_test.h"
void* DataProcess(void* args) {
  int32_t socket_fd = *reinterpret_cast<int32_t*>(args);
  char buffer[1024];
  while (read(socket_fd, buffer, 1024)) {
    printf("arrived:%s\n", buffer);
    memset(buffer, 0, 1024);
  }
  return NULL;
}
int32_t SocketBindAndListen(int32_t port, char* hostname) {
  int32_t fd = socket(AF_INET, SOCK_STREAM, 0);
  PrintError(fd == -1);
  struct sockaddr_in socket_addr;
  memset(&socket_addr, 0, sizeof(socket_addr));
  socket_addr.sin_family = AF_INET;
  socket_addr.sin_port = htons(port);
  socket_addr.sin_addr.s_addr = inet_addr(hostname);
  if (INADDR_NONE == socket_addr.sin_addr.s_addr) {
    fprintf(stderr, "host name is error!\n");
    return -1;
  }
  int32_t ret = bind(fd, (struct sockaddr*)&socket_addr, sizeof(sockaddr_in));
  PrintError(ret != 0);
  ret = listen(fd, 10);
  PrintError(ret != 0);
  socklen_t sockaddr_in_len = sizeof(struct sockaddr_in);
  for (;;) {
    int32_t client_socket =
        accept(fd, (struct sockaddr*)&socket_addr, &sockaddr_in_len);
    PrintError(client_socket == -1);
    pthread_t tid;
    ret = pthread_create(&tid, NULL, DataProcess, &client_socket);
    PrintError(ret != 0);
    ret = pthread_detach(tid);
    PrintError(ret != 0);
  }
  return 0;
}
int32_t NetProcess(int32_t port, char* hostname) {
  int32_t ret = SocketBindAndListen(port, hostname);
  if (ret != 0) {
    fprintf(stderr, "SocketBindAndListen failed\n");
    return -1;
  }
  return 0;
}
int32_t main(int32_t argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "argv error:openssl_test_server address port !\n");
    return -1;
  }
  int32_t port = atoi(argv[2]);
  if (port == 0) {
    fprintf(stderr, "args error:port should be number!\n");
    return -1;
  }
  int32_t ret = NetProcess(port, argv[1]);
  if (ret != 0) {
    fprintf(stderr, "NetProcess failed\n");
    return -1;
  }
  return 0;
}
