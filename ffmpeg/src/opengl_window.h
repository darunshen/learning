/*
 * Created on Mon Oct 21 2019
 * Author : tianrun
 * Copyright (c) 2019
 */
#ifndef FFMPEG_SRC_OPENGL_WINDOW_H_
#define FFMPEG_SRC_OPENGL_WINDOW_H_
#include <map>
#include "data_type.h"
using std::function;
using std::map;
class OpenglWindow {
 public:
  OpenglWindow(int argc, char *argv[], struct play_info *const info,
               function<int32_t(void)> init_function);

 private:
  int32_t InitMaps();
  int32_t InitCoordinateAndTextureTarget();
  int32_t ReadFileToBuffer(const string file_name, string &buffer);
  int32_t InstallShaderWithProgram(const struct shader_files);
  static void Display(void);
  static void LoopFunctionPerFrame(int32_t value);
  struct play_info *p_info_;
  map<enum AVPixelFormat, struct shader_files> pixel_format_shader_map_;
};
#endif  // FFMPEG_SRC_OPENGL_WINDOW_H_
