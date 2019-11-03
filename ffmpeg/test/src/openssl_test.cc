/*
 * Created on Sun Nov 03 2019
 * Copyright (c) 2019
 */
#include "openssl_test.h"
SSL_CTX* InitSSLCtx(char* ca_filename, char* server_cert_file,
                    char* private_key) {
  if (ca_filename == NULL) {
    fprintf(stderr, "ca file name is null !\n");
    return NULL;
  }
  /* registers the available SSL/TLS ciphers and digests */
  SSL_library_init();
  /* adds all algorithms to the table (digests and ciphers) */
  OpenSSL_add_all_algorithms();
  /* registers the error strings for all libcrypto and libssl functions */
  SSL_load_error_strings();
  /**
   * @brief creates a new SSL_CTX object as framework to establish TLS/SSL or
   * DTLS enabled connections ,using SSL_CTX_free() to free, TLS_method() is the
   * general-purpose version-flexible SSL/TLS methods.  The actual protocol
   * version used will be negotiated to the highest version mutually supported
   * by the client and the server.  The supported protocols are SSLv3, TLSv1,
   * TLSv1.1, TLSv1.2 and TLSv1.3.  Applications should use these methods, and
   * avoid the version- specific methods described below.
   */
  SSL_CTX* ctx = SSL_CTX_new(TLS_method());
  if (ctx == NULL) {
    ERR_print_errors_fp(stdout);
    return NULL;
  }
  if (server_cert_file != NULL && private_key != NULL) {
    /* loads the first certificate stored in server_cert_file into ctx. */
    if (SSL_CTX_use_certificate_file(ctx, server_cert_file, SSL_FILETYPE_PEM) <=
        0) {
      ERR_print_errors_fp(stdout);
      return NULL;
    }
    /* adds the first private key found in private_key to ctx. */
    if (SSL_CTX_use_PrivateKey_file(ctx, private_key, SSL_FILETYPE_PEM) <= 0) {
      ERR_print_errors_fp(stdout);
      return NULL;
    }
    /* checks the consistency of a private key with the corresponding
       certificate loaded into ctx */
    if (!SSL_CTX_check_private_key(ctx)) {
      ERR_print_errors_fp(stdout);
      return NULL;
    }
  } else {
    /* sets the verification flags SSL_VERIFY_PEER for ctx to be mode and
     * specifies the verify_callback function NULL to be used. This requires to
     * verify server's cert.
     */
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
  }
  /**
   * @brief specifies the locations for ctx, at which CA certificates for
   * verification purposes are located. The certificates available via
   * CAfile(ca_filename) and CApath are trusted.
   */
  if (!SSL_CTX_load_verify_locations(ctx, ca_filename, NULL)) {
    ERR_print_errors_fp(stdout);
    return NULL;
  }
  return ctx;
}
SSL* CreateSSLConnection(SSL_CTX* ssl_ctx, int32_t socket_fd, bool if_server) {
  /**
   * @brief creates a new SSL structure which is needed to hold the data for a
   * TLS/SSL connection.using SSL_free() to free it
   */
  SSL* ssl = SSL_new(ssl_ctx);
  /* sets the file descriptor socket_fd as the input/output facility for the
   * TLS/SSL (encrypted) side of ssl */
  SSL_set_fd(ssl, socket_fd);
  if (if_server) {
    /* waits for a TLS/SSL client to initiate the TLS/SSL handshake.The
       communication channel must already have been set and assigned to the ssl
       by setting an underlying BIO. */
    if (SSL_accept(ssl) == -1) {
      perror("accept");
      close(socket_fd);
      return NULL;
    }
  } else {
    /**
     * @brief initiates the TLS/SSL handshake with a server. The communication
     * channel must already have been set and assigned to the ssl by setting an
     * underlying BIO.
     */
    if (SSL_connect(ssl) == -1) {
      ERR_print_errors_fp(stderr);
    } else {
      printf("ssl connected %s\n", SSL_get_cipher(ssl));
    }
  }
  return ssl;
}
