//
// Created by wangrl2016 on 2023/4/4.
//

#pragma once

#include <memory>

namespace media {
    // A specialized buffer for interfacing with audio/video decoders.
    class DecoderBuffer {
    public:
        // Allocates buffer with |size| >= 0.
        explicit DecoderBuffer(size_t size);

        DecoderBuffer(const DecoderBuffer&) = delete;

        DecoderBuffer& operator=(const DecoderBuffer&) = delete;

        [[nodiscard]] const uint8_t* data() const {
            return data_.get();
        }

        [[nodiscard]] size_t data_size() const {
            return size_;
        }

        // Create a DecoderBuffer whose |data_| is copied from |data|. |data| must not
        // be nullptr and |size| >= 0.
        static std::shared_ptr<DecoderBuffer> CopyFrom(const uint8_t* data,
                                                       size_t size);

        // Create a DecoderBuffer where data() of |size| bytes resides within the heap
        // as byte array.
        // Ownership of |data| is transferred to the buffer.
        static std::shared_ptr<DecoderBuffer> FromArray(std::unique_ptr<uint8_t[]> data,
                                                        size_t size);
    protected:
        DecoderBuffer(const uint8_t* data, size_t size);

        DecoderBuffer(std::unique_ptr<uint8_t[]> data, size_t size);

        // Encoded data, if it is stored on the heap.
        std::unique_ptr<uint8_t[]> data_;

    private:
        // Constructor helper method for memory allocations.
        void Initialize();

        // Size of the encoded data.
        size_t size_;

    };
}
