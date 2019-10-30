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
class MediaPlay {
 public:
  MediaPlay(play_info* info);
  virtual ~MediaPlay();
  int32_t StartDecoding();

 private:
  int32_t Decoding();
  int32_t DecodeContextInit();
  int32_t InitDecoderContext(int* stream_idx, AVCodecContext** dec_ctx,
                             AVFormatContext* fmt_ctx, enum AVMediaType type);
  int32_t DecodePacket(int* got_frame, int cached, AVPacket pkt);
  play_info* info;
  AVFormatContext* fmt_ctx = NULL;
  AVCodecContext* video_dec_ctx = NULL;
  AVFrame* frame = NULL;
};
#endif  // FFMPEG_SRC_MEDIA_PLAY_H_
