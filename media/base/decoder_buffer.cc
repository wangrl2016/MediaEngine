//
// Created by wangrl2016 on 2023/4/4.
//

#include <glog/logging.h>
#include "media/base/decoder_buffer.h"

namespace media {
    void DecoderBuffer::Initialize() {
        data_.reset(new uint8_t[size_]);
    }

    DecoderBuffer::DecoderBuffer(size_t size) : size_(size) {
        Initialize();
    }

    DecoderBuffer::DecoderBuffer(const uint8_t* data, size_t size) :
            size_(size) {
        if (!data) {
            CHECK_EQ(size_, 0);
            return;
        }

        Initialize();

        memcpy(data_.get(), data, size_);
    }

    DecoderBuffer::DecoderBuffer(std::unique_ptr<uint8_t[]> data, size_t size) :
            data_(std::move(data)), size_(size) {}

    std::shared_ptr<DecoderBuffer> DecoderBuffer::CopyFrom(const uint8_t* data,
                                                           size_t size) {
        return std::shared_ptr<DecoderBuffer>(new DecoderBuffer(data, size));
    }

    std::shared_ptr<DecoderBuffer> DecoderBuffer::FromArray(std::unique_ptr<uint8_t[]> data,
                                                            size_t size) {
        CHECK(data);
        return std::shared_ptr<DecoderBuffer>(new DecoderBuffer(std::move(data), size));
    }
}
