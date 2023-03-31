//
// Created by wangrl2016 on 2023/3/29.
//

#include "media/filters/ffmpeg_glue.h"

namespace media {
    FFmpegGlue::FFmpegGlue(const std::shared_ptr<FFmpegURLProtocol>& protocol) {
        format_context_ = avformat_alloc_context();
    }

    FFmpegGlue::~FFmpegGlue() {

    }

    bool FFmpegGlue::OpenContext() {

    }
}
