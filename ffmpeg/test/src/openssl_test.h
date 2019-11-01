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
#endif  // FFMPEG_TEST_SRC_OPENSSL_TEST_H_
