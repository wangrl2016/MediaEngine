//
// Created by wangrl2016 on 2023/3/29.
//

#pragma once

#include <cstdint>

namespace base {
    class TimeDelta {
    public:
        constexpr TimeDelta() = default;

    private:
        // Delta in microseconds.
        int64_t delta_ = 0;
    };
}

