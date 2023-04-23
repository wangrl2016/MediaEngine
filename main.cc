//
// Created by wangrl2016 on 2023/3/27.
//

#include <cstdlib>

#include <gflags/gflags.h>

DEFINE_string(intput, "", "lottie animation to render");
DEFINE_string(output, "", "mp4 file to create");
DEFINE_string(asset, "", "path to assets needed for json file");
DEFINE_int32(fps, 25, "video frames per second");

int main(int argc, char* argv[]) {
    return EXIT_SUCCESS;
}
