/*
 * Created on Mon Oct 21 2019
 * Author : tianrun
 * Copyright (c) 2019
 */
#include "opengl_window.h"
#include <GL/freeglut.h>
#include <GLES3/gl3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using std::ifstream;
using std::stringstream;
struct play_info* OpenglWindow::p_info_ = NULL;
static struct shader_data {
  GLuint uniform_location_yuv_y;
  GLuint uniform_location_yuv_u;
  GLuint uniform_location_yuv_v;
  GLuint texture_target_yuv_y;
  GLuint texture_target_yuv_u;
  GLuint texture_target_yuv_v;
  const GLuint vertex_layout_location = 0;
  const GLuint texture_layout_location = 1;
  const GLfloat vertex_coordinates[8] = {
      -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
  };
  const GLfloat texture_coordinates[8] = {
      0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
  };
} shader_data_store;
OpenglWindow::OpenglWindow(int argc, char* argv[], struct play_info* const info,
                           function<int32_t(void)> init_function) {
  if (0 != InitMaps()) {
    fprintf(stderr, "InitMaps failed\n");
    exit(1);
  }

  OpenglWindow::p_info_ = info;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowPosition(info->pos_x, info->pos_y);
  glutInitWindowSize(info->width, info->height);
  // glutInitWindowSize(800, 600);
  glutCreateWindow(info->window_name.c_str());

  glLoadIdentity();
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glutDisplayFunc(Display);
  glutTimerFunc(40, LoopFunctionPerFrame, 0);
  if (0 != InstallShaderWithProgram(
               this->pixel_format_shader_map_.at(info->video_info.pix_fmt))) {
    fprintf(stderr, "InstallShaderWithProgram failed\n");
    exit(1);
  }
  if (init_function() != 0) {
    fprintf(stderr, "init function failed\n");
    exit(1);
  }
  glutMainLoop();
}
int32_t OpenglWindow::InitMaps() {
  struct shader_files init_sf;
  init_sf.vert_file_name = "./Yuv420ToRgb.vert";
  init_sf.frag_file_name = "./Yuv420ToRgb.frag";
  pixel_format_shader_map_[AVPixelFormat::AV_PIX_FMT_YUV420P] = init_sf;
  return 0;
}
int32_t OpenglWindow::InitCoordinateAndTextureTarget() {
  glVertexAttribPointer(shader_data_store.vertex_layout_location, 2, GL_FLOAT,
                        0, 0, shader_data_store.vertex_coordinates);
  glEnableVertexAttribArray(shader_data_store.vertex_layout_location);
  glVertexAttribPointer(shader_data_store.texture_layout_location, 2, GL_FLOAT,
                        0, 0, shader_data_store.texture_coordinates);
  glEnableVertexAttribArray(shader_data_store.texture_layout_location);
  glGenTextures(1, &shader_data_store.texture_target_yuv_y);
  glBindTexture(GL_TEXTURE_2D, shader_data_store.texture_target_yuv_y);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glGenTextures(1, &shader_data_store.texture_target_yuv_u);
  glBindTexture(GL_TEXTURE_2D, shader_data_store.texture_target_yuv_u);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glGenTextures(1, &shader_data_store.texture_target_yuv_v);
  glBindTexture(GL_TEXTURE_2D, shader_data_store.texture_target_yuv_v);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void OpenglWindow::LoopFunctionPerFrame(int32_t value) {
  Display();
  glutTimerFunc(40, LoopFunctionPerFrame, 0);
}
void OpenglWindow::Display(void) {
  if (p_info_ == NULL || p_info_->decoded_frame_buffer.size() == 0) {
    return;
  }
  p_info_->decoded_frame_buffer_mutex.lock();
  uint8_t* vedio_frame = p_info_->decoded_frame_buffer.front();
  p_info_->decoded_frame_buffer.pop_front();
  p_info_->decoded_frame_buffer_mutex.unlock();
  glClearColor(0.0, 0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, shader_data_store.texture_target_yuv_y);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, p_info_->width, p_info_->height, 0,
               GL_RED, GL_UNSIGNED_BYTE, vedio_frame);
  glUniform1i(shader_data_store.uniform_location_yuv_y, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, shader_data_store.texture_target_yuv_u);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, p_info_->width / 2,
               p_info_->height / 2, 0, GL_RED, GL_UNSIGNED_BYTE,
               vedio_frame + p_info_->width * p_info_->height);
  glUniform1i(shader_data_store.uniform_location_yuv_u, 1);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, shader_data_store.texture_target_yuv_v);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, p_info_->width / 2,
               p_info_->height / 2, 0, GL_RED, GL_UNSIGNED_BYTE,
               vedio_frame + p_info_->width * p_info_->height * 5 / 4);
  glUniform1i(shader_data_store.uniform_location_yuv_v, 2);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glutSwapBuffers();
  free(vedio_frame);
}
int32_t OpenglWindow::ReadFileToBuffer(const string file_name, string& buffer) {
  ifstream file_stream;
  stringstream string_stream;
  file_stream.open(file_name, ifstream::in);
  if (!file_stream.is_open()) {
    fprintf(stderr, "Error opening file!\n");
    return 1;
  }
  string_stream << file_stream.rdbuf();
  file_stream.close();
  buffer = string_stream.str();
  return 0;
}
int32_t OpenglWindow::InstallShaderWithProgram(
    const struct shader_files shader_info) {
  GLuint vertex_shader, fragment_shader, program;
  string vertex_shader_code_string, fragment_shader_code_string;
  GLint success;
  // read shader code from shader files and compile them
  ReadFileToBuffer(shader_info.vert_file_name, vertex_shader_code_string);
  ReadFileToBuffer(shader_info.frag_file_name, fragment_shader_code_string);
  const GLchar* const vertex_shader_code_char =
      vertex_shader_code_string.c_str();
  const GLchar* const fragment_shader_code_char =
      fragment_shader_code_string.c_str();
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_code_char, NULL);
  glShaderSource(fragment_shader, 1, &fragment_shader_code_char, NULL);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (GL_TRUE != success) {
    GLchar logs[512];
    glGetShaderInfoLog(vertex_shader, 512, NULL, logs);
    fprintf(stderr, "vertex_shader glCompileShader failed:%s\n", logs);
    return 1;
  }
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (GL_TRUE != success) {
    GLchar logs[512];
    glGetShaderInfoLog(fragment_shader, 512, NULL, logs);
    fprintf(stderr, "fragment_shader glCompileShader failed:%s\n", logs);
    return 1;
  }
  // use program to link the shaders
  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (GL_TRUE != success) {
    GLchar logs[512];
    glGetProgramInfoLog(program, 512, NULL, logs);
    fprintf(stderr, "glLinkProgram failed:%s\n", logs);
    return 1;
  }
  // delete shader object which already linked to out program
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glUseProgram(program);
  shader_data_store.uniform_location_yuv_y =
      glGetUniformLocation(program, "yuv_y");
  shader_data_store.uniform_location_yuv_u =
      glGetUniformLocation(program, "yuv_u");
  shader_data_store.uniform_location_yuv_v =
      glGetUniformLocation(program, "yuv_v");
  return 0;
}
