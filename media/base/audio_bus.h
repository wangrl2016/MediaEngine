//
// Created by wangrl2016 on 2023/4/23.
//

#pragma once

#include <memory>
#include "media/base/audio_parameters.h"

namespace media {
    // Represents a sequence of audio frames containing frames() audio samples fro
    // each of channels() channels. The data is stored as a set of contiguous
    // float arrays with one array per channel. The memory for the arrays is either
    // allocated and owned by the AudioBus or it is provided to one of the factory
    // methods.
    class AudioBus {
    public:
        // Guaranteed alignment of each channel's data; use 16-byte alignment for easy
        // SSE optimizations.
        enum { kChannelAlignment = 16 };

        static std::unique_ptr<AudioBus> Create(int channels, int frames);

        static std::unique_ptr<AudioBus> Create(const AudioParameters& params);

    protected:
        AudioBus(int channels, int frames);
    };
}
