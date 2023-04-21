//
// Created by wangrl2016 on 2023/4/21.
//

#pragma once

#include <cstdint>

namespace base {
    class TimeBase {
    public:
        static constexpr int64_t kHoursPerDay = 24;
        static constexpr int64_t kSecondsPerMinute = 60;
        static constexpr int64_t kMinutesPerHour = 60;
        static constexpr int64_t kSecondsPerHour =
                kSecondsPerMinute * kMinutesPerHour;
        static constexpr int64_t kMillisecondsPerSecond = 1000;
        static constexpr int64_t kMillisecondsPerDay =
                kMillisecondsPerSecond * kSecondsPerHour * kHoursPerDay;
        static constexpr int64_t kMicrosecondsPerMillisecond = 1000;
        static constexpr int64_t kMicrosecondsPerSecond =
                kMicrosecondsPerMillisecond * kMillisecondsPerSecond;
        static constexpr int64_t kMicrosecondsPerMinute =
                kMicrosecondsPerSecond * kSecondsPerMinute;
        static constexpr int64_t kMicrosecondsPerHour =
                kMicrosecondsPerMinute * kMinutesPerHour;
        static constexpr int64_t kMicrosecondsPerDay =
                kMicrosecondsPerHour * kHoursPerDay;
        static constexpr int64_t kMicrosecondsPerWeek = kMicrosecondsPerDay * 7;
        static constexpr int64_t kNanosecondsPerMicrosecond = 1000;
        static constexpr int64_t kNanosecondsPerSecond =
                kNanosecondsPerMicrosecond * kMicrosecondsPerSecond;
    };
}
