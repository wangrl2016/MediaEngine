//
// Created by admin on 2023/4/11.
//

#pragma once

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}

namespace media {
    struct ScopedPtrAVFree {
        inline void operator()(void* x) const {
            av_free(x);
        }
    };

    struct ScopedPtrFreeAVCodecContext {
        inline void operator()(void* x) const {
            auto* codec_context = static_cast<AVCodecContext*>(x);
            avcodec_free_context(&codec_context);
        }
    };

    struct ScopedPtrFreeSwrContext {
        inline void operator()(void* x) const {
            auto* swr_context = static_cast<SwrContext*>(x);
            swr_free(&swr_context);
        }
    };
}
