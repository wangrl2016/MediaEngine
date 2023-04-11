//
// Created by wangrl2016 on 2023/3/27.
//

#include <memory>
#include "media/filters/audio_file_reader.h"

namespace media {
    AudioFileReader::AudioFileReader(std::shared_ptr<FFmpegURLProtocol> protocol) :
            stream_index_(0),
            protocol_(protocol) {}

    AudioFileReader::~AudioFileReader() {
        Close();
    }

    bool AudioFileReader::Open() {
        return OpenDemuxer() && OpenDecoder();
    }

    bool AudioFileReader::SetOutputParameters(const AudioParameters& parameters) {

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

    base::TimeDelta AudioFileReader::GetDuration() const {

    }

    int AudioFileReader::GetOutputNumberOfFrames() const {

    }

    bool AudioFileReader::OpenDemuxer() {
        glue_ = std::make_unique<FFmpegGlue>(protocol_);
        AVFormatContext* format_context = glue_->format_context();

        // Open FFmpeg AVFormatContext.
        if (!glue_->OpenContext()) {

        }
    }

    bool AudioFileReader::OpenDecoder() {

    }

    bool AudioFileReader::ReadPacket(AVPacket* output_packet) {

    }
}
