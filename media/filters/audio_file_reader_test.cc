//
// Created by wangrl2016 on 2023/4/4.
//

#include <gtest/gtest.h>
#include "media/base/decoder_buffer.h"

namespace media {
    class AudioFileReaderTest : public testing::Test {
    public:

    private:
        std::shared_ptr<DecoderBuffer> data_;
    };

    TEST(AudioFileReaderTest, WithoutOpen) {

    }
}