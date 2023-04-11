//
// Created by admin on 2023/4/11.
//

#pragma once

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

namespace media {
    struct ScopedPtrAVFree {
        inline void operator()(void* x) const {
            av_free(x);
        }
    };

    struct ScopedPtrAVFreeContext {
        inline void operator()(void* x) const {
            auto* codec_context = static_cast<AVCodecContext*>(x);
            avcodec_free_context(&codec_context);
        }
    };
}
