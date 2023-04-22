//
// Created by wangrl2016 on 2023/4/4.
//

#include <memory>
#include <gtest/gtest.h>
#include "media/base/test_data_util.h"
#include "media/base/decoder_buffer.h"
#include "media/filters/memory_url_protocol.h"
#include "media/filters/audio_file_reader.h"

namespace media {
    class AudioFileReaderTest : public testing::Test {
    public:
        AudioFileReaderTest() = default;

        AudioFileReaderTest(const AudioFileReaderTest&) = delete;

        AudioFileReaderTest& operator=(const AudioFileReaderTest&) = delete;

        ~AudioFileReaderTest() override = default;

        void Initialize(const char* filename) {
            data_ = ReadTestDataFile(filename);
            protocol_ = std::make_shared<MemoryUrlProtocol>(
                    data_->data(), data_->data_size());
            reader_ = std::make_unique<AudioFileReader>(protocol_);
        }

        void RunTest(const char* filename,
                     const char* hash,
                     int channels,
                     int sample_rate,
                     base::TimeDelta duration,
                     int frames,
                     int expected_frames) {
            Initialize(filename);
            ASSERT_TRUE(reader_->Open());
            EXPECT_EQ(channels, reader_->InputChannels());
            EXPECT_EQ(sample_rate, reader_->InputSampleRate());
        }

    private:
        std::shared_ptr<DecoderBuffer> data_;
        std::shared_ptr<MemoryUrlProtocol> protocol_;
        std::unique_ptr<AudioFileReader> reader_;
    };

    TEST_F(AudioFileReaderTest, WithoutOpen) {
        Initialize("Feel-Good.mp3");
    }

    TEST_F(AudioFileReaderTest, MP3) {
        RunTest("Feel-Good.mp3",
                nullptr,
                2,
                44100,
                base::Microseconds(0),
                0,
                0);
    }
}