//
// Created by wangrl2016 on 2023/3/29.
//

#pragma once

#include <numeric>
#include <cstdint>
#include "base/time/time_base.h"

namespace base {
    class TimeDelta {
    public:
        constexpr TimeDelta() = default;

        constexpr explicit TimeDelta(int64_t delta) : delta_(delta) {}

        static constexpr TimeDelta FromInternalValue(int64_t delta) {
            return TimeDelta(delta);
        }

        [[nodiscard]] constexpr bool is_inf() const {
            return delta_ == std::numeric_limits<int64_t>::infinity();
        }

        [[nodiscard]] constexpr double InSecondsF() const;

    private:
        // Delta in microseconds.
        int64_t delta_ = 0;
    };

    template<typename T>
    constexpr TimeDelta Microseconds(T n) {
        return TimeDelta::FromInternalValue(n);
    }

    constexpr double TimeDelta::InSecondsF() const {
        if (!is_inf())
            return static_cast<double>(delta_) / TimeBase::kMicrosecondsPerSecond;
        return (delta_ < 0) ? -std::numeric_limits<double>::infinity()
                : std::numeric_limits<double>::infinity();
    }
}
