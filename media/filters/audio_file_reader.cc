//
// Created by wangrl2016 on 2023/3/27.
//

#include <memory>
#include <utility>
#include <glog/logging.h>
#include "media/filters/audio_file_reader.h"
#include "media/ffmpeg/ffmpeg_common.h"

namespace media {
    // AAC(M4A) decoding specific constants.
    static const int kAACPrimingFrameCount = 2112;
    static const int kAACRemainderFrameCount = 519;

    AudioFileReader::AudioFileReader(std::shared_ptr<FFmpegURLProtocol> protocol) :
            stream_index_(0),
            protocol_(std::move(protocol)) {}

    AudioFileReader::~AudioFileReader() {
        Close();
    }

    bool AudioFileReader::Open() {
        return OpenDemuxer() && OpenDecoder();
    }

    bool AudioFileReader::SetOutputParameters(const AudioParameters& parameters) {
        output_audio_parameters_ = parameters;

        if (input_audio_parameters_.channel_count() != output_audio_parameters_.channel_count() ||
                input_audio_parameters_.sample_rate() != output_audio_parameters_.sample_rate() ||
                input_audio_parameters_.av_sample_format() != output_audio_parameters_.av_sample_format()) {
            // Initialize the resample to be able to convert audio sample formats.
            swr_context_ = std::unique_ptr<SwrContext, ScopedPtrFreeSwrContext>(
                    swr_alloc_set_opts(
                            nullptr,
                            av_get_default_channel_layout(output_audio_parameters_.channel_count()),
                            output_audio_parameters_.av_sample_format(),
                            output_audio_parameters_.sample_rate(),
                            av_get_default_channel_layout(input_audio_parameters_.channel_count()),
                            input_audio_parameters_.av_sample_format(),
                            input_audio_parameters_.sample_rate(),
                            0,
                            nullptr));

            if (!swr_context_ || swr_init(swr_context_.get()) < 0) {
                LOG(ERROR) << "Could not init swr context";
                Close();
                return false;
            }
        }

        return true;
    }

    void AudioFileReader::Close() {
        codec_context_.reset();
        swr_context_.reset();
        glue_.reset();
    }

    int AudioFileReader::Read(std::vector<std::unique_ptr<AudioBus>>* decoded_audio_packets,
                              int packets_to_read) {
        return 0;
    }

    bool AudioFileReader::HasKnownDuration() const {
        return glue_->format_context()->duration != AV_NOPTS_VALUE;
    }

    base::TimeDelta AudioFileReader::GetDuration() const {
        const AVRational av_time_base = {1, AV_TIME_BASE};

        DCHECK_NE(glue_->format_context()->duration, AV_NOPTS_VALUE);
        int64_t estimated_duration_us = glue_->format_context()->duration;

        if (input_audio_parameters_.av_codec_id() == AV_CODEC_ID_AAC) {
            // For certain AAC-encoded files, FFMPEG's estimated frame count might not
            // be sufficient to capture the entire audio content that we want. This is
            // especially noticeable for short files (< 10ms) resulting in silence
            // throughout the decoded buffer. Thus, we add the priming frames and the
            // remainder frames to the estimation.
            estimated_duration_us += ceil(
                    1000000.0 * static_cast<double>(kAACPrimingFrameCount + kAACRemainderFrameCount) /
                    InputSampleRate());
        } else {
            // Add one microsecond to avoid rounding-down errors which can occur when
            // |duration| has been calculated from an exact number of sample-frames.
            // One microsecond is much less than the time of a single sample-frame
            // at any real-world sample-rate.
            estimated_duration_us += 1;
        }

        return ConvertFromTimeBase(av_time_base,
                                   estimated_duration_us);
    }

    int AudioFileReader::GetOutputNumberOfFrames() const {
        return std::ceil(GetDuration().InSecondsF() * output_audio_parameters_.sample_rate());
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
        codec_context_ = std::unique_ptr<AVCodecContext, ScopedPtrFreeAVCodecContext>(
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
