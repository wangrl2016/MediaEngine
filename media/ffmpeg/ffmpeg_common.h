//
// Created by wangrl2016 on 2023/4/21.
//

#pragma once

extern "C" {
#include <libavutil/mathematics.h>
#include <libavutil/rational.h>
}
#include "base/time/time_delta.h"

namespace media {
    base::TimeDelta ConvertFromTimeBase(const AVRational& time_base,
                                        int64_t timestamp);
}
