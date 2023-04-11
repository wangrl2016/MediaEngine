//
// Created by wangrl2016 on 2023/3/29.
//

#include "media/base/audio_parameters.h"

namespace media {
    AudioParameters::AudioParameters() :
            AudioParameters(0, 0, AV_SAMPLE_FMT_NONE) {}

    AudioParameters::AudioParameters(int channel_count,
                                     int sample_rate,
                                     AVSampleFormat format) :
            channel_count_(channel_count),
            sample_rate_(sample_rate),
            av_sample_format_(format) {}

    AudioParameters::~AudioParameters() = default;
}
