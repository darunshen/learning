cmake_minimum_required(VERSION 3.2)
# include_directories(${PROJECT_SOURCE_DIR}/test/openssl/include})
# link_directories(${PROJECT_SOURCE_DIR}/test/openssl/lib)
link_libraries(/usr/lib/x86_64-linux-gnu/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/test)
SET(LIB_LIST  avformat avdevice avcodec avutil swscale swresample
pthread va-drm va va-x11 va m va Xv X11 Xext x265 x264 z lzma bz2)

ADD_EXECUTABLE(demuxing_decoding ${PROJECT_SOURCE_DIR}/test/src/demuxing_decoding.c)
TARGET_LINK_LIBRARIES(demuxing_decoding ${LIB_LIST} )

ADD_EXECUTABLE(opengl_test ${PROJECT_SOURCE_DIR}/test/src/opengl_test.cc)
TARGET_LINK_LIBRARIES(opengl_test ${LIB_LIST}  GL glut)

ADD_EXECUTABLE(openssl_test_client ${PROJECT_SOURCE_DIR}/test/src/openssl_test.cc ${PROJECT_SOURCE_DIR}/test/src/openssl_test_client.cc)
TARGET_LINK_LIBRARIES(openssl_test_client ssl crypto dl)

ADD_EXECUTABLE(openssl_test_server ${PROJECT_SOURCE_DIR}/test/src/openssl_test.cc ${PROJECT_SOURCE_DIR}/test/src/openssl_test_server.cc)
TARGET_LINK_LIBRARIES(openssl_test_server ssl crypto pthread dl)

file(COPY ${PROJECT_SOURCE_DIR}/test/openssl/bin/openssl 
    DESTINATION ${PROJECT_SOURCE_DIR}/build/test)
file(COPY ${PROJECT_SOURCE_DIR}/test/openssl/ssl/misc/CA.sh 
    DESTINATION ${PROJECT_SOURCE_DIR}/build/test)
file(COPY ${PROJECT_SOURCE_DIR}/test/openssl/data/server.crt
    DESTINATION ${PROJECT_SOURCE_DIR}/build/test)
file(COPY ${PROJECT_SOURCE_DIR}/test/openssl/data/server.key
    DESTINATION ${PROJECT_SOURCE_DIR}/build/test)
file(COPY ${PROJECT_SOURCE_DIR}/test/openssl/data/ca.crt
    DESTINATION ${PROJECT_SOURCE_DIR}/build/test)
