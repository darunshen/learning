/*
 * Created on Fri Nov 01 2019
 * Copyright (c) 2019
 */
#include <stdio.h>
#include "openssl_test.h"
SSL_CTX* ssl_ctx = NULL;
int32_t SocketConnect(int32_t port, char* hostname) {
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
  int32_t ret =
      connect(fd, (struct sockaddr*)&socket_addr, sizeof(sockaddr_in));
  PrintError(ret != 0);
  SSL* ssl = CreateSSLConnection(ssl_ctx, fd, false);
  if (ssl == NULL) {
    fprintf(stderr, "CreateSSLConnection failed,socket_fd = %d\n", fd);
    return -1;
  }
  char buffer[1024];
  while (1) {
    errno = 0;
    ret = scanf("%s", buffer);
    PrintError(errno != 0);
    ret = SSL_write(ssl, buffer, 1024);
    PrintError(ret == -1);
  }
  SSL_shutdown(ssl);
  SSL_free(ssl);
  close(fd);
  return 0;
}
int32_t NetProcess(int32_t port, char* hostname) {
  int32_t ret = SocketConnect(port, hostname);
  if (ret != 0) {
    fprintf(stderr, "SocketConnect failed\n");
    return -1;
  }
  return 0;
}
int32_t main(int32_t argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr,
            "argv error:openssl_test_server address port ca_filename!\n");
    return -1;
  }
  int32_t port = atoi(argv[2]);
  if (port == 0) {
    fprintf(stderr, "args error:port should be number!\n");
    return -1;
  }
  ssl_ctx = InitSSLCtx(argv[3]);
  if (ssl_ctx == NULL) {
    fprintf(stderr, "InitSSLCtx failed\n");
    return -1;
  }
  int32_t ret = NetProcess(port, argv[1]);
  if (ret != 0) {
    fprintf(stderr, "NetProcess failed\n");
    return -1;
  }
  SSL_CTX_free(ssl_ctx);
  return 0;
}
