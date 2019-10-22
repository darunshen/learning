/*
 * Created on Thu Oct 17 2019
 * Author : tianrun
 * Copyright (c) 2019
 */
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include "data_type.h"
#include "media_play.h"
#include "opengl_window.h"
using std::bind;
int main(int argc, char *argv[]) {
  struct play_info trans_data;
  trans_data.window_name = string("media_player");
  // trans_data.pos_x = 10;
  // trans_data.pos_y = 10;
  trans_data.video_info.uri =
      "/home/bigrain/Documents/code/my_project/learning/ffmpeg/build/55.mp4";
  MediaPlay mp(&trans_data);
  OpenglWindow ow(argc, argv, &trans_data, bind(&MediaPlay::StartDecoding, mp));
  return 0;
}
