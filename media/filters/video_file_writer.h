//
// Created by wangrl2016 on 2023/4/24.
//

#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
}

#include "base/memory/noncopyable.h"

namespace media {
    // a wrapper around a single output AVStream
    typedef struct OutputStream {
        AVStream* stream;
        AVCodecContext* encoder;

        // pts of the next frame that will be generated
        int64_t next_pts;
        int samples_count;

        AVFrame* frame;
        AVFrame* tmp_frame;

        AVPacket* tmp_pkt;

        float t, cr, cr2;

        struct SwsContext* sws_ctx;
        struct SwrContext* swr_ctx;
    } OutputStream;

    class VideoFileWriter : public base::Noncopyable {
    public:
        VideoFileWriter();

        ~VideoFileWriter();

        bool Open(std::string filename);

        bool WriteTest();

        void Close();

    private:
        // Add an output stream.
        void AddStream(OutputStream* stream, const AVCodec** codec, enum AVCodecID codec_id);

        void OpenVideo(AVDictionary* opt_arg);

        void OpenAudio(AVDictionary* opt_arg);

        // Video output.
        AVFrame* AllocVideoFrame(enum AVPixelFormat pix_fmt, int width, int height);

        AVFrame* AllocAudioFrame(enum AVSampleFormat sample_fmt,
                                 uint64_t channel_layout,
                                 int sample_rate,
                                 int nb_samples);

        int WriteFrame(AVFormatContext* output_format_context,
                       AVCodecContext* codec_context,
                       AVStream* stream,
                       AVFrame* frame,
                       AVPacket* pkt);

        int WriteVideoFrameTest(AVFormatContext* output_format_context,
                                OutputStream* ost);

        int WriteAudioFrameTest(AVFormatContext* output_format_context,
                                OutputStream* ost);

        AVFrame* GetVideoFrameTest();

        AVFrame* GetAudioFrameTest();

        void FillYUVImageTest(AVFrame* pict,
                              int frame_index,
                              int width,
                              int height);


    private:
        OutputStream video_stream_, audio_stream_;
        AVFormatContext* output_format_context_;
        const AVCodec* audio_codec_, * video_codec_;

        bool have_video = false, have_audio = false;
        bool encode_video = false, encode_audio = false;
    };
}
