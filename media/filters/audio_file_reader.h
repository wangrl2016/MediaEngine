//
// Created by wangrl2016 on 2023/3/27.
//

#pragma once

#include <vector>

extern "C" {
#include <libavformat/avformat.h>
}

#include "base/memory/noncopyable.h"
#include "base/time/time_delta.h"
#include "media/base/audio_parameters.h"
#include "media/ffmpeg/ffmpeg_delete.h"
#include "media/filters/ffmpeg_glue.h"

namespace media {
    class AudioBus;

    class FFmpegURLProtocol;

    class AudioFileReader : public base::Noncopyable {
    public:
        // Audio file data will be read using the given protocol.
        // The AudioFileReader does not take ownership of |protocol| and
        // simply maintains a weak reference to it.
        explicit AudioFileReader(std::shared_ptr<FFmpegURLProtocol> protocol);

        virtual ~AudioFileReader();

        // Open() reads the audio data format so that the sample_rate(),
        // channels(), GetDuration(), and GetNumberOfFrames() methods can be called.
        // It returns |true| on success.
        bool Open();

        bool SetOutputParameters(const AudioParameters& parameters);

        std::shared_ptr<AudioBus> Read();

        void Close();

        // These methods can ba called once Open() has been called.
        [[nodiscard]] int InputChannels() const { return input_audio_parameters_.channel_count(); }

        [[nodiscard]] int InputSampleRate() const { return input_audio_parameters_.sample_rate(); }

        // Returns true if (an estimated) duration of the audio data is
        // known. Must be called after Open().
        [[nodiscard]] bool HasKnownDuration() const;

        [[nodiscard]] base::TimeDelta GetDuration() const;

        [[nodiscard]] int GetOutputNumberOfFrames() const;

    private:
        bool OpenDemuxer();

        bool OpenDecoder();

        // Destruct |glue_| after |codec_context_|.
        std::unique_ptr<FFmpegGlue> glue_;
        std::unique_ptr<AVCodecContext, ScopedPtrFreeAVCodecContext> codec_context_;
        std::unique_ptr<SwrContext, ScopedPtrFreeSwrContext> swr_context_;

        int stream_index_;
        std::shared_ptr<FFmpegURLProtocol> protocol_;

        AudioParameters input_audio_parameters_;
        AudioParameters output_audio_parameters_;
    };
}
