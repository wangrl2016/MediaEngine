//
// Created by wangrl2016 on 2023/3/29.
//

#pragma once

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
}

namespace media {
    class AudioParameters {
    public:
        AudioParameters();

        AudioParameters(int channel_count,
                        int sample_rate,
                        AVSampleFormat format);

        ~AudioParameters();

        [[nodiscard]] int channel_count() const { return channel_count_; }

        void set_channel_count(int channel_count) { channel_count_ = channel_count; }

        [[nodiscard]] int sample_rate() const { return sample_rate_; }

        void set_sample_rate(int sample_rate) { sample_rate_ = sample_rate; }

        [[nodiscard]] AVSampleFormat av_sample_format() const { return av_sample_format_; }

        void set_av_sample_format(AVSampleFormat format) {
            av_sample_format_ = format;
        }

    private:
        int channel_count_;
        int sample_rate_;
        AVSampleFormat av_sample_format_;
    };
}

