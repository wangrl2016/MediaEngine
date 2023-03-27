//
// Created by wangrl2016 on 2023/3/27.
//

#include "media/filters/audio_file_reader.h"

namespace media {
    AudioFileReader::AudioFileReader(FFmpegURLProtocol* protocol) {

    }

    AudioFileReader::~AudioFileReader() {
        Close();
    }

    bool AudioFileReader::Open() {
        return true;
    }

    void AudioFileReader::Close() {

    }

    int AudioFileReader::Read(std::vector<std::unique_ptr<AudioBus>>* decoded_audio_packets,
                              int packets_to_read) {
        return 0;
    }

    bool AudioFileReader::HasKnownDuration() const {
        return true;
    }
}
