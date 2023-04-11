//
// Created by admin on 2023/4/11.
//

#pragma once

extern "C" {
#include <libavformat/avformat.h>
}

namespace media {
    struct ScopedPtrAVFree {
        inline void operator()(void* x) const {
            av_free(x);
        }
    };
}
