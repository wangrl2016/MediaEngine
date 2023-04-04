//
// Created by wangrl2016 on 2023/3/27.
//

#include "media/filters/audio_file_reader.h"

namespace media {
    AudioFileReader::AudioFileReader(FFmpegURLProtocol* protocol) :
            stream_index_(0),
            protocol_(protocol),
            channels_(0),
            sample_rate_(0),
            av_sample_format_(AV_SAMPLE_FMT_NONE) {}

    AudioFileReader::~AudioFileReader() {
        Close();
    }

    bool AudioFileReader::Open() {
        return OpenDemuxer() && OpenDecoder();
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

    base::TimeDelta AudioFileReader::GetDuration() const {

    }

    int AudioFileReader::GetNumberOfFrames() const {

    }

    bool AudioFileReader::OpenDemuxer() {

    }

    bool AudioFileReader::OpenDecoder() {

    }

    bool AudioFileReader::ReadPacket(AVPacket* output_packet) {

    }
}
