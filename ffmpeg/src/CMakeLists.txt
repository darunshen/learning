cmake_minimum_required(VERSION 3.2)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/src)
SET(LIB_LIST  avformat avdevice avcodec avutil swscale swresample
pthread va-drm va va-x11 va m va Xv X11 Xext x265 x264 z lzma bz2 GL glut)

ADD_EXECUTABLE(media_player ${PROJECT_SOURCE_DIR}/src/main.cc 
    ${PROJECT_SOURCE_DIR}/src/media_play.cc ${PROJECT_SOURCE_DIR}/src/opengl_window.cc)
TARGET_LINK_LIBRARIES(media_player ${LIB_LIST} )
