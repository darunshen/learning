/*
 * Created on Fri Nov 01 2019
 * Copyright (c) 2019
 */
#ifndef FFMPEG_TEST_SRC_OPENSSL_TEST_H_
#define FFMPEG_TEST_SRC_OPENSSL_TEST_H_
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define PrintError(if_error) \
  {                          \
    if (if_error) {          \
      strerror(errno);       \
      return -1;             \
    }                        \
  }
/**
 * @brief 创建ssl上下文
 *
 * @param ca_filename ca证书
 * @param server_cert_file 若不为null则代表ssl server端的证书
 * @param private_key 若不为null则代表ssl server端的非对称加密密钥
 * @return SSL_CTX*
 */
SSL_CTX* InitSSLCtx(char* ca_filename, char* server_cert_file = NULL,
                    char* private_key = NULL);
/**
 * @brief 创建根据已建立的tcp连接建立ssl连接
 *
 * @param ssl_ctx ssl上下文
 * @param socket_fd 已连接的tcp socket句柄
 * @param if_server 是否为server端程序
 * @return SSL* tls/ssl 连接，使用SSL_free()来释放
 */
SSL* CreateSSLConnection(SSL_CTX* ssl_ctx, int32_t socket_fd, bool if_server);
#endif  // FFMPEG_TEST_SRC_OPENSSL_TEST_H_
