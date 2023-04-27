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
            AddStream(&video_stream_, &video_codec_, fmt->video_codec);
            have_video = true;
            encode_video = true;
        }
        if (fmt->audio_codec != AV_CODEC_ID_NONE) {
            AddStream(&audio_stream_, &audio_codec_, fmt->audio_codec);
            have_audio = true;
            encode_audio = true;
        }

        // Now that all the parameters are set, we can open the audio and
        // video codecs and allocate the necessary encode buffers.
        if (have_video) {
            OpenVideo(nullptr);
        }

        if (have_audio) {
            OpenAudio(nullptr);
        }

        av_dump_format(output_format_context_, 0, filename.c_str(), 1);

        int ret;
        // Open the output file, if needed.
        if (!(fmt->flags & AVFMT_NOFILE)) {
            ret = avio_open(&output_format_context_->pb, filename.c_str(), AVIO_FLAG_WRITE);
            if (ret < 0) {
                LOG(ERROR) << "Could not open " << filename << ", err " << ret;
                return false;
            }
        }

        // Write the stream header, if any.
        ret = avformat_write_header(output_format_context_, nullptr);

        return true;
    }

    bool VideoFileWriter::WriteTest() {
        while (encode_video || encode_audio) {
            if (!encode_audio || av_compare_ts(video_stream_.next_pts,
                                               video_stream_.encoder->time_base,
                                               audio_stream_.next_pts,
                                               audio_stream_.encoder->time_base) <= 0) {
                encode_video = !WriteVideoFrameTest(output_format_context_, &video_stream_);
            } else {
                encode_audio = !WriteAudioFrameTest(output_format_context_, &audio_stream_);
            }
        }
        av_write_trailer(output_format_context_);
    }

    void VideoFileWriter::Close() {

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
                codec_context->sample_fmt = (*codec)->sample_fmts ?
                                            (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
                codec_context->bit_rate = 64000;
                codec_context->sample_rate = 44100;
                if ((*codec)->supported_samplerates) {
                    codec_context->sample_rate = (*codec)->supported_samplerates[0];
                    for (i = 0; (*codec)->supported_samplerates[i]; i++) {
                        if ((*codec)->supported_samplerates[i] == 44100)
                            codec_context->sample_rate = 44100;
                    }
                }
                codec_context->channel_layout = AV_CH_LAYOUT_STEREO;
                stream->stream->time_base = (AVRational) {1, codec_context->sample_rate};
                break;
            case AVMEDIA_TYPE_VIDEO:
                codec_context->codec_id = codec_id;
                codec_context->bit_rate = 400000;
                // Resolution must be a multiple of two.
                codec_context->width = 352;
                codec_context->height = 288;
                // Timebase: This is the fundamental unit of time (in seconds) in terms
                // of which frame timestamps are represented. For fixed-fps content,
                // timebase should be 1 / framerate and timestamp increments should be
                // identical to 1.
                stream->stream->time_base = (AVRational) {1, 25};
                codec_context->time_base = stream->stream->time_base;
                codec_context->gop_size = 12;   // emit one intra frame every twelve frames at most
                codec_context->pix_fmt = AV_PIX_FMT_YUV420P;
                if (codec_context->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
                    // just for testing, we also add B-frames
                    codec_context->max_b_frames = 2;
                }
                if (codec_context->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
                    // Needed to avoid using macro-blocks in which some coefficient overflow.
                    // This does not happen with normal video, it just happens here as
                    // the motion of the chroma plane does not match the luma plane.
                    codec_context->mb_decision = 2;
                }
                break;

            default:
                break;
        }

        // Some formats want stream headers to be separate.
        if (output_format_context_->flags & AVFMT_GLOBALHEADER)
            codec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    void VideoFileWriter::OpenVideo(AVDictionary* opt_arg) {
        int ret;
        AVCodecContext* codec_context = video_stream_.encoder;
        AVDictionary* opt = nullptr;
        av_dict_copy(&opt, opt_arg, 0);

        // open the codec
        ret = avcodec_open2(codec_context, video_codec_, &opt);
        av_dict_free(&opt);
        if (ret < 0) {
            LOG(ERROR) << "Could not open video codec: " << ret;
            return;
        }

        // allocate and init a re-usable frame
        video_stream_.frame = AllocVideoFrame(codec_context->pix_fmt,
                                              codec_context->width,
                                              codec_context->height);
        if (!video_stream_.frame) {
            LOG(INFO) << "Could not allocate video frame";
            return;
        }

        // If the output format is not YUV420P, then a temporary YUV420P
        // picture is needed too. It is then converted to the required
        // output format.
        video_stream_.tmp_frame = nullptr;
        if (codec_context->pix_fmt != AV_PIX_FMT_YUV420P) {
            video_stream_.tmp_frame = AllocVideoFrame(AV_PIX_FMT_YUV420P,
                                                      codec_context->width,
                                                      codec_context->height);
            if (!video_stream_.tmp_frame) {
                LOG(ERROR) << "Could not allocate temporary video frame";
                return;
            }
        }

        // copy the stream parameters to the muxer
        ret = avcodec_parameters_from_context(video_stream_.stream->codecpar, codec_context);
        if (ret < 0) {
            LOG(ERROR) << "Could not copy the stream parameters";
            return;
        }
    }

    void VideoFileWriter::OpenAudio(AVDictionary* opt_arg) {
        AVCodecContext* codec_context;
        int nb_samples;
        int ret;
        AVDictionary* opt = nullptr;
        codec_context = audio_stream_.encoder;

        // open it
        av_dict_copy(&opt, opt_arg, 0);
        ret = avcodec_open2(codec_context, audio_codec_, &opt);
        av_dict_free(&opt);
        if (ret < 0) {
            LOG(ERROR) << "Could not open audio codec: " << ret;
            return;
        }

        // init signal generator
        audio_stream_.t = 0;
        audio_stream_.cr = 2 * M_PI * 110.0 / codec_context->sample_rate;
        // increment frequency by 110 Hz per second
        audio_stream_.cr2 = 2 * M_PI * 110.0 / codec_context->sample_rate / codec_context->sample_rate;

        if (codec_context->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
            nb_samples = 10000;
        else
            nb_samples = codec_context->frame_size;

        audio_stream_.frame = AllocAudioFrame(codec_context->sample_fmt,
                                              codec_context->channel_layout,
                                              codec_context->sample_rate,
                                              nb_samples);
        audio_stream_.tmp_frame = AllocAudioFrame(AV_SAMPLE_FMT_S16,
                                                  codec_context->channel_layout,
                                                  codec_context->sample_rate,
                                                  nb_samples);

        // Copy the stream parameters to the muxer.
        ret = avcodec_parameters_from_context(audio_stream_.stream->codecpar, codec_context);
        if (ret < 0) {
            LOG(ERROR) << "Could not copy the stream parameters";
            return;
        }

        // Create resampler context.
        audio_stream_.swr_ctx = swr_alloc_set_opts(nullptr,
                                                   int64_t(codec_context->channel_layout),
                                                   codec_context->sample_fmt,
                                                   codec_context->sample_rate,
                                                   int64_t(codec_context->channel_layout),
                                                   AV_SAMPLE_FMT_S16,
                                                   codec_context->sample_rate,
                                                   0,
                                                   nullptr);
        if (!audio_stream_.swr_ctx) {
            LOG(ERROR) << "Could not allocate resampler context";
            return;
        }

        // Initialize the resampling context.
        if ((ret = swr_init(audio_stream_.swr_ctx)) < 0) {
            LOG(ERROR) << "Failed to initialize the resampling context, ret " << ret;
        }
    }

    AVFrame* VideoFileWriter::AllocVideoFrame(enum AVPixelFormat pix_fmt, int width, int height) {
        AVFrame* frame;
        int ret;
        frame = av_frame_alloc();
        if (!frame)
            return nullptr;

        frame->format = pix_fmt;
        frame->width = width;
        frame->height = height;

        // allocate the buffers for the frame data
        ret = av_frame_get_buffer(frame, 0);
        if (ret < 0) {
            LOG(ERROR) << "Could not allocate frame data";
            return nullptr;
        }
        return frame;
    }

    AVFrame* VideoFileWriter::AllocAudioFrame(enum AVSampleFormat sample_fmt,
                                              uint64_t channel_layout,
                                              int sample_rate,
                                              int nb_samples) {
        AVFrame* frame = av_frame_alloc();
        if (!frame) {
            LOG(ERROR) << "Error allocating an audio frame";
            return nullptr;
        }

        frame->format = sample_fmt;
        frame->channel_layout = channel_layout;
        frame->sample_rate = sample_rate;
        frame->nb_samples = nb_samples;

        if (nb_samples) {
            if (av_frame_get_buffer(frame, 0) < 0) {
                LOG(ERROR) << "Error allocating an audio buffer";
                return nullptr;
            }
        }
        return frame;
    }

    int VideoFileWriter::WriteFrame(AVFormatContext* output_format_context,
                                AVCodecContext* codec_context,
                                AVStream* stream,
                                AVFrame* frame,
                                AVPacket* pkt) {
        int ret;

        // send the frame to the encoder
        ret = avcodec_send_frame(codec_context, frame);
    }

    int VideoFileWriter::WriteVideoFrameTest(AVFormatContext* output_format_context, OutputStream* ost) {

    }

    int VideoFileWriter::WriteAudioFrameTest(AVFormatContext* output_format_context, OutputStream* ost) {}
}
