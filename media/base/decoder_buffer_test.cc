//
// Created by wangrl2016 on 2023/4/22.
//

#include <gtest/gtest.h>
#include "media/base/decoder_buffer.h"

namespace media {
    TEST(DecoderBufferTest, Constructors) {
        std::shared_ptr<DecoderBuffer> buffer1(new DecoderBuffer(0));
        EXPECT_TRUE(buffer1->data());
        EXPECT_EQ(0u, buffer1->data_size());

        const size_t kTestSize = 10;
        std::shared_ptr<DecoderBuffer> buffer2(new DecoderBuffer(kTestSize));
        ASSERT_TRUE(buffer2.get());
        EXPECT_EQ(kTestSize, buffer2->data_size());
    }

    TEST(DecoderBufferTest, CopyFrom) {
        const uint8_t kData[] = "hello";
        const size_t kDataSize = std::size(kData);

        std::shared_ptr<DecoderBuffer> buffer1(DecoderBuffer::CopyFrom(
                reinterpret_cast<const uint8_t*>(&kData), kDataSize));
        ASSERT_TRUE(buffer1.get());
        EXPECT_NE(kData, buffer1->data());
        EXPECT_EQ(buffer1->data_size(), kDataSize);
        EXPECT_EQ(0, memcmp(buffer1->data(), kData, kDataSize));
    }

    TEST(DecoderBufferTest, FromArray) {
        const uint8_t kData[] = "hello";
        const size_t kDataSize = std::size(kData);
        auto ptr = std::make_unique<uint8_t[]>(kDataSize);
        memcpy(ptr.get(), kData, kDataSize);

        std::shared_ptr<DecoderBuffer> buffer(
                DecoderBuffer::FromArray(std::move(ptr), kDataSize));
        ASSERT_TRUE(buffer.get());
        EXPECT_EQ(buffer->data_size(), kDataSize);
        EXPECT_EQ(0, memcmp(buffer->data(), kData, kDataSize));
    }
}
