cmake_minimum_required(VERSION 3.2)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/test)
SET(LIB_LIST  avformat avdevice avcodec avutil swscale swresample
pthread va-drm va va-x11 va m va Xv X11 Xext x265 x264 z lzma bz2)

ADD_EXECUTABLE(demuxing_decoding ${PROJECT_SOURCE_DIR}/test/src/demuxing_decoding.c)
TARGET_LINK_LIBRARIES(demuxing_decoding ${LIB_LIST} )
