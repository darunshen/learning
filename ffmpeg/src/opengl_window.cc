/*
 * Created on Mon Oct 21 2019
 * Author : tianrun
 * Copyright (c) 2019
 */
#include "opengl_window.h"
#include <GL/freeglut.h>
#include <iostream>
#include <vector>

OpenglWindow::OpenglWindow(int argc, char *argv[],
                           const struct play_info *const info,
                           function<int32_t(void)> init_function) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowPosition(info->pos_x, info->pos_y);
  glutInitWindowSize(info->width, info->height);
  glutCreateWindow(info->window_name.c_str());

  glLoadIdentity();
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glutDisplayFunc(Display);
  if (init_function() != 0) {
    fprintf(stderr, "init function failed\n");
    exit(1);
  }
  glutMainLoop();
}

void OpenglWindow::Display(void) {}
