//
// Created by wangrl2016 on 2023/3/27.
//

#include <glog/logging.h>
#include "media/filters/audio_file_writer.h"

namespace media {
    AudioFileWriter::AudioFileWriter() : pts_(0) {

    }

    AudioFileWriter::~AudioFileWriter() {

    }

    bool AudioFileWriter::Open(const std::string& file_path,
                               const AudioParameters& input_audio_parameters,
                               const AudioParameters& output_audio_parameters) {
        int error;

        return true;
    }

    void AudioFileWriter::Close() {

    }
}
