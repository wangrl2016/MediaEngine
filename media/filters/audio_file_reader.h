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
#include "media/filters/ffmpeg_glue.h"

namespace media {
    class AudioBus;

    class FFmpegURLProtocol;

    class AudioFileReader : public base::Noncopyable {
    public:
        // Audio file data will be read using the given protocol.
        // The AudioFileReader does not take ownership of |protocol| and
        // simply maintains a weak reference to it.
        explicit AudioFileReader(FFmpegURLProtocol* protocol);

        virtual ~AudioFileReader();

        // Open() reads the audio data format so that the sample_rate(),
        // channels(), GetDuration(), and GetNumberOfFrames() methods can be called.
        // It returns |true| on success.
        bool Open();

        void Close();

        int Read(std::vector<std::unique_ptr<AudioBus>>* decoded_audio_packets,
                 int packets_to_read = std::numeric_limits<int>::max());

        // These methods can ba called once Open() has been called.
        [[nodiscard]] int channels() const { return channels_; }

        [[nodiscard]] int sample_rate() const { return sample_rate_; }

        // Returns true if (an estimated) duration of the audio data is
        // known. Must be called after Open().
        [[nodiscard]] bool HasKnownDuration() const;

        [[nodiscard]] base::TimeDelta GetDuration() const;

        [[nodiscard]] int GetNumberOfFrames() const;

    private:
        bool OpenDemuxer();

        bool OpenDecoder();

        bool ReadPacket(AVPacket* output_packet);

        // Destruct |glue_| after |codec_context_|.
        std::unique_ptr<FFmpegGlue> glue_;

        int stream_index_;
        FFmpegURLProtocol* protocol_;

        int channels_;
        int sample_rate_;
        AVSampleFormat av_sample_format_;
    };
}
