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
OpenglWindow::OpenglWindow(int argc, char* argv[], struct play_info* const info,
                           function<int32_t(void)> init_function) {
  if (0 != InitMaps()) {
    fprintf(stderr, "InitMaps failed\n");
    exit(1);
  }

  this->p_info_ = info;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowPosition(info->pos_x, info->pos_y);
  // glutInitWindowSize(info->width, info->height);
  glutInitWindowSize(800, 600);
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
int32_t OpenglWindow::InitCoordinateAndTextureTarget() {}
void OpenglWindow::LoopFunctionPerFrame(int32_t value) {
  Display();
  glutTimerFunc(40, LoopFunctionPerFrame, 0);
}
void OpenglWindow::Display(void) {}
int32_t OpenglWindow::ReadFileToBuffer(const string file_name, string& buffer) {
  ifstream file_stream;
  stringstream string_stream;
  file_stream.open(file_name, ifstream::in);
  if (!file_stream.is_open()) {
    std::cout << "Error opening file";
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
    std::cout << "vertex_shader glCompileShader failed:\n" << logs << std::endl;
    return 1;
  }
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (GL_TRUE != success) {
    GLchar logs[512];
    glGetShaderInfoLog(fragment_shader, 512, NULL, logs);
    std::cout << "fragment_shader glCompileShader failed:\n"
              << logs << std::endl;
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
    std::cout << "glLinkProgram failed\n" << logs << std::endl;
    return 1;
  }
  // delete shader object which already linked to out program
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glUseProgram(program);
  return 0;
}
