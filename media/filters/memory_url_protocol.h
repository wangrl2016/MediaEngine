//
// Created by wangrl2016 on 2023/3/31.
//

#pragma once

#include "media/filters/ffmpeg_url_protocol.h"
#include "base/memory/noncopyable.h"

namespace media {
    // Simple FFmpegURLProtocol that reads from a buffer.
    // NOTE: This object does not copy the buffer so the buffer pointer passed into the
    // constructor needs to remain valid for the entire lifetime of this object.
    class MemoryUrlProtocol : public FFmpegURLProtocol, base::Noncopyable {
    public:
        MemoryUrlProtocol(const uint8_t* data, int64_t size);

        virtual ~MemoryUrlProtocol();

        // FFmpegURLProtocol methods.
        int Read(int size, uint8_t* data) override;

        bool GetPosition(int64_t* position_out) override;

        bool SetPosition(int64_t position) override;

        bool GetSize(int64_t* size_out) override;

    private:
        const uint8_t* data_;
        int64_t size_;
        int64_t position_;
    };
}
