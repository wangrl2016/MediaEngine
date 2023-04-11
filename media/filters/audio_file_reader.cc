//
// Created by wangrl2016 on 2023/3/27.
//

#include <memory>
#include <glog/logging.h>
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
            DLOG(WARNING) << "AudioFileReader::Open() : error in avformat_open_input()";
            return false;
        }

        const int result = avformat_find_stream_info(format_context, nullptr);
        if (result < 0) {
            DLOG(WARNING) << "AudioFileReader::Open() : error in avformat_find_stream_info()";
            return false;
        }

        // Calling avformat_find_stream_info can uncover new streams. We wait till now
        // to find the first audio stream, if any.
        codec_context_.reset();
        bool found_stream = false;
        for (size_t i = 0; i < format_context->nb_streams; ++i) {
            if (format_context->streams[i]->codecpar->codec_type ==
                AVMEDIA_TYPE_AUDIO) {
                stream_index_ = int(i);
                found_stream = true;
                break;
            }
        }

        if (!found_stream)
            return false;
    }

    bool AudioFileReader::OpenDecoder() {

    }

    bool AudioFileReader::ReadPacket(AVPacket* output_packet) {

    }
}
