//
// Created by wangrl2016 on 2023/3/27.
//

#pragma once

#include <string>
#include "base/memory/noncopyable.h"
#include "media/base/audio_parameters.h"

namespace media {
    class AudioFileWriter : public base::Noncopyable {
    public:
        AudioFileWriter();

        ~AudioFileWriter();

        bool Open(const std::string& file_path,
                  const AudioParameters& input_audio_parameters,
                  const AudioParameters& output_audio_parameters);

        void Close();

    private:
        // Timestamp for the audio frames.
        int64_t pts_;
        AudioParameters input_audio_parameters_;
        AudioParameters output_audio_parameters_;

        AVFormatContext* output_format_context_ = nullptr;
    };
}
