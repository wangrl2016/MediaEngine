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

        // get the codec context
        codec_context_ = std::unique_ptr<AVCodecContext, ScopedPtrAVFreeContext>(
                avcodec_alloc_context3(nullptr));
        if (avcodec_parameters_to_context(codec_context_.get(),
                                          format_context->streams[stream_index_]->codecpar) < 0) {
            return false;
        }

        DCHECK_EQ(codec_context_->codec_type, AVMEDIA_TYPE_AUDIO);
        return true;
    }

    bool AudioFileReader::OpenDecoder() {
        const AVCodec* codec = avcodec_find_decoder(codec_context_->codec_id);
        if (codec) {
            // Mp3 decodes to S16P which we don't support, tell it to use S16 instead.
            if (codec_context_->sample_fmt == AV_SAMPLE_FMT_S16P)
                codec_context_->request_sample_fmt = AV_SAMPLE_FMT_S16;

            const int result = avcodec_open2(codec_context_.get(), codec, nullptr);
            if (result < 0) {
                DLOG(WARNING) << "AudioFileReader::Open(): could not open codec - result: " << result;
                return false;
            }

            // Ensure avcodec_open2() respected our format request.
            if (codec_context_->sample_fmt == AV_SAMPLE_FMT_S16P) {
                DLOG(ERROR) << "AudioFileReader::Open() : unable to configure a"
                            << " supported sample format - "
                            << codec_context_->sample_fmt;
                return false;
            }
        } else {
            DLOG(WARNING) << "AudioFileReader::Open() : could not find codec.";
            return false;
        }

        // Store initial values to guard against midstream configuration changes.
        input_audio_parameters_.set_channel_count(codec_context_->channels);
        input_audio_parameters_.set_sample_rate(codec_context_->sample_rate);
        input_audio_parameters_.set_av_sample_format(codec_context_->sample_fmt);
        return true;
    }

    bool AudioFileReader::ReadPacket(AVPacket* output_packet) {

    }
}
