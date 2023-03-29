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
    private:
        AVSampleFormat av_sample_format_;
    };
}

