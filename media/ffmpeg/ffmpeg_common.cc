//
// Created by wangrl2016 on 2023/4/21.
//

#include "base/time/time_base.h"
#include "media/ffmpeg/ffmpeg_common.h"


namespace media {
    static const AVRational kMicrosBase = { 1, base::TimeBase::kMicrosecondsPerSecond };

    base::TimeDelta ConvertFromTimeBase(const AVRational& time_base,
                                        int64_t timestamp) {
        int64_t microseconds = av_rescale_q(timestamp, time_base, kMicrosBase);
        return base::Microseconds(microseconds);
    }
}
