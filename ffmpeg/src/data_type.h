/*
 * Created on Mon Oct 21 2019
 * Copyright (c) 2019
 */

#ifndef FFMPEG_SRC_DATA_TYPE_H_
#define FFMPEG_SRC_DATA_TYPE_H_
#include <libavutil/pixfmt.h>
#include <stdint.h>
#include <functional>
#include <list>
#include <mutex>
#include <string>
using std::list;
using std::mutex;
using std::string;
enum VideoInputType { FILE_TYPE = 0, RTMP_TYPE };
struct play_info {
  int width;          /**< 窗口宽度 */
  int height;         /**< 窗口长度 */
  int pos_x;          /**< 窗口位置 */
  int pos_y;          /**< 窗口位置 */
  string window_name; /**< 窗口名字 */
  struct video_info {
    string uri;                          /**< 视频输入地址 */
    VideoInputType video_intput_type;    /**< 视频输入类型 */
    int32_t video_stream_index;          /**< 视频流索引 */
    enum AVPixelFormat pix_fmt;          /**< 视频像素格式 */
    uint8_t *video_dst_data[4] = {NULL}; /**< 存储解码数据 */
    int32_t video_dst_linesize[4];       /**< 存储解码数据间距 */
    int32_t video_dst_bufsize;           /**< 视频帧大小 */
  } video_info;                          /**< 视频输入信息 */
  list<uint8_t *> decoded_frame_buffer;  /**< 已解码buffer的双向列表 */
  mutex decoded_frame_buffer_mutex; /**< 已解码buffer的双向列表的锁 */
};
/**
 * @brief 存储yuv通道数据
 *
 */
struct yuv_channel_info {
  int32_t width, height;
  uint8_t *data;
};
/**
 * @brief 存储shader文件名称
 *
 */
struct shader_files {
  string frag_file_name; /**< 片段着色器文件名 */
  string vert_file_name; /**< 顶点着色器文件名 */
};

#endif  // FFMPEG_SRC_DATA_TYPE_H_
