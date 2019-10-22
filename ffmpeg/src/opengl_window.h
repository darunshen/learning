/*
 * Created on Mon Oct 21 2019
 * Author : tianrun
 * Copyright (c) 2019
 */
#ifndef FFMPEG_SRC_OPENGL_WINDOW_H_
#define FFMPEG_SRC_OPENGL_WINDOW_H_
#include "data_type.h"
using std::function;
class OpenglWindow {
 public:
  OpenglWindow(int argc, char *argv[], const struct play_info *const info,
               function<int32_t(void)> init_function);

 private:
  static void Display(void);
  static void LoopFunctionPerFrame(int32_t value);
};
#endif  // FFMPEG_SRC_OPENGL_WINDOW_H_
