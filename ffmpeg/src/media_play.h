/*
 * Created on Mon Oct 21 2019
 * Copyright (c) 2019
 */
#ifndef FFMPEG_SRC_MEDIA_PLAY_H_
#define FFMPEG_SRC_MEDIA_PLAY_H_
#include "data_type.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/timestamp.h"
#ifdef __cplusplus
}
#endif
/**
 * @brief provide media stream process feature
 *
 */
class MediaPlay {
 public:
  /**
   * @brief Construct a new Media Play object,and init format context and decode
   * context for the input info
   *
   * @param info
   */
  MediaPlay(play_info* info);
  virtual ~MediaPlay();
  /**
   * @brief start decoding , this can be call outside this class
   *
   * @return int32_t
   */
  int32_t StartDecoding();

 private:
  /**
   * @brief is call with a thread by StartDecoding
   *
   * @return int32_t
   */
  int32_t Decoding();
  /**
   * @brief create context for decoding and alloc space for frame(for decoding)
   * and image(yuv), called in MediaPlay(play_info* info)
   *
   * @return int32_t
   */
  int32_t DecodeContextInit();
  /**
   * @brief create decode context and init it for corresponding codec id
   *
   * @param[in] stream_idx stream id
   * @param[out] dec_ctx decode context
   * @param[in] fmt_ctx format context
   * @param[in] type stream type , must be video
   * @return int32_t
   */
  int32_t InitDecoderContext(int* stream_idx, AVCodecContext** dec_ctx,
                             AVFormatContext* fmt_ctx, enum AVMediaType type);
  /**
   * @brief decode one frame from one packet and push it back in queue
   *
   * @param[out] got_frame if decode one frame
   * @param[in] cached if the frame in cache
   * @param[in] pkt packet contains one frame
   * @return int32_t
   */
  int32_t DecodePacket(int* got_frame, int cached, AVPacket pkt);
  play_info* info;
  AVFormatContext* fmt_ctx = NULL;
  AVCodecContext* video_dec_ctx = NULL;
  AVFrame* frame = NULL;
};
#endif  // FFMPEG_SRC_MEDIA_PLAY_H_
