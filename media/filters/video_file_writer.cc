//
// Created by wangrl2016 on 2023/4/24.
//

#include <glog/logging.h>

#include "media/filters/video_file_writer.h"

namespace media {
    VideoFileWriter::VideoFileWriter() {

    }

    VideoFileWriter::~VideoFileWriter() {

    }

    bool VideoFileWriter::Open(std::string filename) {
        // allocate the output media context
        avformat_alloc_output_context2(&output_format_context_, nullptr, nullptr, filename.c_str());
        if (!output_format_context_) {
            LOG(INFO) << "Could not deduce output format from file extension: using MPEG.";
            avformat_alloc_output_context2(&output_format_context_, nullptr, "mpeg", filename.c_str());
        }
        if (!output_format_context_)
            return false;

        const AVOutputFormat* fmt;

        fmt = output_format_context_->oformat;

        // Add the audio and video stream using the default format codecs
        // and initialize the codecs.
        if (fmt->video_codec != AV_CODEC_ID_NONE) {

        }
    }

    void VideoFileWriter::AddStream(OutputStream* stream, const AVCodec** codec, enum AVCodecID codec_id) {
        AVCodecContext* codec_context;
        int i;

        // find the encoder
        *codec = avcodec_find_encoder(codec_id);
        if (!(*codec)) {
            LOG(ERROR) << "Could not find encoder for " << avcodec_get_name(codec_id);
            return;
        }

        stream->tmp_pkt = av_packet_alloc();
        if (!stream->tmp_pkt) {
            LOG(ERROR) << "Could not allocate AVPacket";
            return;
        }

        stream->stream = avformat_new_stream(output_format_context_, nullptr);
        if (!stream->stream) {
            LOG(ERROR) << "Could not alloc an encoding context";
            return;
        }
        stream->stream->id = int(output_format_context_->nb_streams - 1);
        codec_context = avcodec_alloc_context3(*codec);
        if (!codec_context) {
            LOG(ERROR) << "Could not alloc an encoding context";
            return;
        }
        stream->encoder = codec_context;

        switch ((*codec)->type) {
            case AVMEDIA_TYPE_AUDIO:

                break;
            case AVMEDIA_TYPE_VIDEO:
                break;

            default:
                break;
        }
    }
}
