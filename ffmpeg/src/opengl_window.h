/*
 * Created on Mon Oct 21 2019
 * Copyright (c) 2019
 */
#ifndef FFMPEG_SRC_OPENGL_WINDOW_H_
#define FFMPEG_SRC_OPENGL_WINDOW_H_
#include <map>
#include "data_type.h"
using std::function;
using std::map;
/**
 * @brief provide window display and rendering feature
 *
 */
class OpenglWindow {
 public:
  /**
   * @brief Construct a new Opengl Window object
   *
   * @param argc
   * @param argv
   * @param info
   * @param init_function this function will be called before glutMainLoop()
   */
  OpenglWindow(int argc, char *argv[], struct play_info *const info,
               function<int32_t(void)> init_function);

 private:
  /**
   * @brief init map data for searching shader files with relative pixel format
   *
   * @return int32_t
   */
  int32_t InitMaps();
  /**
   * @brief init opengl's coordinate and generator three texture target for yuv
   *
   * @return int32_t
   */
  int32_t InitCoordinateAndTextureTarget();
  /**
   * @brief read shader file to buffer
   *
   * @param file_name
   * @param buffer
   * @return int32_t
   */
  int32_t ReadFileToBuffer(const string file_name, string &buffer);
  /**
   * @brief install shader using opengl's program
   *
   * @return int32_t
   */
  int32_t InstallShaderWithProgram(const struct shader_files);
  static void Display(void);
  static void LoopFunctionPerFrame(int32_t value);
  static struct play_info *p_info_;
  map<enum AVPixelFormat, struct shader_files> pixel_format_shader_map_;
};
#endif  // FFMPEG_SRC_OPENGL_WINDOW_H_
