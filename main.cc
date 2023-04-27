//
// Created by wangrl2016 on 2023/3/27.
//

#include <cstdlib>

#include <gflags/gflags.h>
#include "media/filters/video_file_writer.h"

DEFINE_string(intput, "", "lottie animation to render");
DEFINE_string(output, "out.mp4", "mp4 file to create");
DEFINE_string(asset, "", "path to assets needed for json file");
DEFINE_int32(fps, 25, "video frames per second");

int main(int argc, char* argv[]) {
    std::unique_ptr<media::VideoFileWriter> writer =
            std::make_unique<media::VideoFileWriter>();

    if (!writer->Open(FLAGS_output)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
