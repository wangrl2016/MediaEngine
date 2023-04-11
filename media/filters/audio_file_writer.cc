//
// Created by wangrl2016 on 2023/3/27.
//

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

        // Open the output file to write to it.


        return true;
    }
}
