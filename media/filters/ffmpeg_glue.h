//
// Created by wangrl2016 on 2023/3/29.
//

#pragma once

#include <memory>
extern "C" {
#include <libavformat/avformat.h>
}
#include "base/memory/noncopyable.h"
#include "media/filters/ffmpeg_url_protocol.h"

namespace media {
    class FFmpegGlue : public base::Noncopyable {
    public:
        explicit FFmpegGlue(const std::shared_ptr<FFmpegURLProtocol>& protocol);

        ~FFmpegGlue();

        // Opens an AVFormatContext specially prepared to process reads and seeks
        // through the FFmpegURLProtocol provided during construction.
        bool OpenContext();

        AVFormatContext* format_context() { return format_context_; }

    private:

        AVFormatContext* format_context_ = nullptr;
    };
}