/*
 * Created on Wed Oct 30 2019
 * Copyright (c) 2019
 */
#ifndef FFMPEG_SRC_UTILTY_HPP_
#define FFMPEG_SRC_UTILTY_HPP_
#include "data_type.h"
/**
 * @brief extract yuv pixel three panel data from frame_data to result
 *
 * @param pix_fmt pixel format
 * @param frame_data a frame data contain lossless yuv data
 * @param width video width
 * @param height video height
 * @param result store the yuv three panel data, [0]-->y [1]-->u [2]-->v
 * @param if_need_free if the result should be free outside
 * @return int32_t 0-->success others-->failed
 */
int32_t YuvPanelDataGet(enum AVPixelFormat pix_fmt, uint8_t* frame_data,
                        int32_t width, int32_t height,
                        struct yuv_panel_info result[3], bool* if_need_free) {
  *if_need_free = false;
  switch (pix_fmt) {
    case AVPixelFormat::AV_PIX_FMT_YUV420P:
      result[0].data = frame_data;
      result[0].width = width;
      result[0].height = height;
      result[1].data = frame_data + width * height;
      result[1].width = width >> 1;
      result[1].height = height >> 1;
      result[2].data = frame_data + width * height * 5 / 4;
      result[2].width = result[1].width;
      result[2].height = result[1].height;
      break;
    default:
      return 1;
  }
  return 0;
}
#endif  // FFMPEG_SRC_UTILTY_HPP_
