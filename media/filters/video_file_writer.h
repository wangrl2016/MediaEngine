//
// Created by wangrl2016 on 2023/4/24.
//

#include <string>
extern "C" {
#include <libavformat/avformat.h>
}
#include "base/memory/noncopyable.h"

namespace media {
    class VideoFileWriter : public base::Noncopyable {
    public:
        VideoFileWriter();

        ~VideoFileWriter();

        bool Open(std::string filename);

    private:
        AVFormatContext* output_format_context_;
    };
}
