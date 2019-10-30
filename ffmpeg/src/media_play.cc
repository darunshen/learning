/*
 * Created on Tue Oct 15 2019
 * Copyright (c) 2019
 */

#include "media_play.h"
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <thread>
using std::thread;
MediaPlay::MediaPlay(play_info* info) : info(info) {
  /* open input file, and allocate format context */
  if (avformat_open_input(&fmt_ctx, info->video_info.uri.c_str(), NULL, NULL) <
      0) {
    fprintf(stderr, "Could not open source uri %s\n",
            info->video_info.uri.c_str());
    exit(1);
  }
  /* retrieve stream information */
  if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
    fprintf(stderr, "Could not find stream information\n");
    exit(1);
  }
  int32_t ret = DecodeContextInit();
  if (ret != 0) {
    fprintf(stderr, "DecodeContextInit Failed\n");
    exit(1);
  }
}
MediaPlay::~MediaPlay() {
  avcodec_free_context(&video_dec_ctx);
  avformat_close_input(&fmt_ctx);
  av_frame_free(&frame);
  av_free(this->info->video_info.video_dst_data[0]);
}
int32_t MediaPlay::DecodeContextInit() {
  int32_t ret = InitDecoderContext(&this->info->video_info.video_stream_index,
                                   &video_dec_ctx, fmt_ctx, AVMEDIA_TYPE_VIDEO);
  if (ret >= 0) {
    AVStream* video_stream =
        fmt_ctx->streams[this->info->video_info.video_stream_index];

    /* allocate image where the decoded image will be put */
    this->info->width = video_dec_ctx->width;
    this->info->height = video_dec_ctx->height;
    this->info->video_info.pix_fmt = video_dec_ctx->pix_fmt;
    int32_t ret = av_image_alloc(this->info->video_info.video_dst_data,
                                 this->info->video_info.video_dst_linesize,
                                 this->info->width, this->info->height,
                                 this->info->video_info.pix_fmt, 1);
    if (ret < 0) {
      fprintf(stderr, "Could not allocate raw video buffer\n");
      return ret;
    }
    this->info->video_info.video_dst_bufsize = ret;
    /* dump input information to stderr */
    av_dump_format(fmt_ctx, 0, this->info->video_info.uri.c_str(), 0);
    if (!video_stream) {
      fprintf(stderr,
              "Could not find audio or video stream in the input, aborting\n");
      ret = 1;
      return ret;
    }
    frame = av_frame_alloc();
    if (!frame) {
      fprintf(stderr, "Could not allocate frame\n");
      ret = AVERROR(ENOMEM);
      return ret;
    }
  }
  return ret;
}
int32_t MediaPlay::StartDecoding() {
  thread one_thread = thread(&MediaPlay::Decoding, this);
  one_thread.detach();
  return 0;
}

int32_t MediaPlay::Decoding() {
  AVPacket pkt;
  int32_t ret;
  /* initialize packet, set data to NULL, let the demuxer fill it */
  av_init_packet(&pkt);
  pkt.data = NULL;
  pkt.size = 0;

  printf("Demuxing video from file '%s'\n", this->info->video_info.uri.c_str());
  int32_t got_frame;
  while (av_read_frame(fmt_ctx, &pkt) >= 0) {
    AVPacket orig_pkt = pkt;
    do {
      ret = DecodePacket(&got_frame, 0, pkt);
      if (ret < 0) break;
      pkt.data += ret;
      pkt.size -= ret;
    } while (pkt.size > 0);
    av_packet_unref(&orig_pkt);
  }

  /* flush cached frames */
  pkt.data = NULL;
  pkt.size = 0;
  do {
    DecodePacket(&got_frame, 1, pkt);
  } while (got_frame);
  return 0;
}
int32_t MediaPlay::DecodePacket(int* got_frame, int cached, AVPacket pkt) {
  if (this->info->decoded_frame_buffer.size() > 30) {
    sleep(1);
  }
  int ret = 0;
  int decoded = pkt.size;
  static int video_frame_count = 0;
  *got_frame = 0;

  if (pkt.stream_index == this->info->video_info.video_stream_index) {
    /* decode video frame */
    ret = avcodec_decode_video2(video_dec_ctx, frame, got_frame, &pkt);
    if (ret < 0) {
      fprintf(stderr, "Error decoding video frame (%s)\n", av_err2str(ret));
      return ret;
    }

    if (*got_frame) {
      if (frame->width != this->info->width ||
          frame->height != this->info->height ||
          frame->format != this->info->video_info.pix_fmt) {
        /* To handle this change, one could call av_image_alloc again and
         * decode the following frames into another rawvideo file. */
        fprintf(stderr,
                "Error: Width, height and pixel format have to be "
                "constant in a rawvideo file, but the width, height or "
                "pixel format of the input video changed:\n"
                "old: width = %d, height = %d, format = %s\n"
                "new: width = %d, height = %d, format = %s\n",
                this->info->width, this->info->height,
                av_get_pix_fmt_name(this->info->video_info.pix_fmt),
                frame->width, frame->height,
                av_get_pix_fmt_name((AVPixelFormat)frame->format));
        return -1;
      }

      printf("video_frame%s n:%d coded_n:%d\n", cached ? "(cached)" : "",
             video_frame_count++, frame->coded_picture_number);

      /* copy decoded frame to destination buffer:
       * this is required since rawvideo expects non aligned data */
      av_image_copy(this->info->video_info.video_dst_data,
                    this->info->video_info.video_dst_linesize,
                    (const uint8_t**)(frame->data), frame->linesize,
                    this->info->video_info.pix_fmt, this->info->width,
                    this->info->height);

      /* add to decoded buffer list */
      this->info->decoded_frame_buffer_mutex.lock();
      uint8_t* frame_buffer = reinterpret_cast<uint8_t*>(
          malloc(info->video_info.video_dst_bufsize * sizeof(uint8_t)));
      memcpy(frame_buffer, this->info->video_info.video_dst_data[0],
             info->video_info.video_dst_bufsize * sizeof(uint8_t));
      this->info->decoded_frame_buffer.push_back(frame_buffer);
      this->info->decoded_frame_buffer_mutex.unlock();
#ifdef TEST_DECODE
      /**
       * @brief generator raw video frame image , use " ffplay  -f rawvideo
       * -video_size 1920x1080( or others ) src/test.dat[%d] " to check it.
       *
       */
      static int32_t s = 0;
      std::ofstream out_file(string("test.dat") + std::to_string(s++),
                             std::ios::out | std::ios::binary);
      out_file.write(reinterpret_cast<char*>(frame_buffer),
                     info->video_info.video_dst_bufsize * sizeof(uint8_t));
      out_file.close();
#endif
    }
  }
  return decoded;
}
int32_t MediaPlay::InitDecoderContext(int* stream_idx, AVCodecContext** dec_ctx,
                                      AVFormatContext* fmt_ctx,
                                      enum AVMediaType type) {
  int32_t ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
  if (ret < 0) {
    fprintf(stderr, "Could not find %s stream in input file '%s'\n",
            av_get_media_type_string(type), this->info->video_info.uri.c_str());
    return ret;
  } else {
    int32_t stream_index = ret;
    AVStream* st = fmt_ctx->streams[stream_index];
    /* find decoder for the stream */
    AVCodec* dec = avcodec_find_decoder(st->codecpar->codec_id);
    if (!dec) {
      fprintf(stderr, "Failed to find %s codec\n",
              av_get_media_type_string(type));
      return AVERROR(EINVAL);
    }
    /* Allocate a codec context for the decoder */
    *dec_ctx = avcodec_alloc_context3(dec);
    if (!*dec_ctx) {
      fprintf(stderr, "Failed to allocate the %s codec context\n",
              av_get_media_type_string(type));
      return AVERROR(ENOMEM);
    }
    /* Copy codec parameters from input stream to output codec context */
    if ((ret = avcodec_parameters_to_context(*dec_ctx, st->codecpar)) < 0) {
      fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
              av_get_media_type_string(type));
      return ret;
    }
    /* Init the decoders*/
    if ((ret = avcodec_open2(*dec_ctx, dec, NULL)) < 0) {
      fprintf(stderr, "Failed to open %s codec\n",
              av_get_media_type_string(type));
      return ret;
    }
    *stream_idx = stream_index;
  }
  return 0;
}
