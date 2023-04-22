//
// Created by wangrl2016 on 2023/4/22.
//

#pragma once
#include "media/base/decoder_buffer.h"

namespace media {
    const char kTestDataPath[] = "resources/audio/";

    std::string GetTestDataFilePath(const std::string& name);

    // Reads a test file from resources/ directory and stores it in a DecoderBuffer.
    std::shared_ptr<DecoderBuffer> ReadTestDataFile(const std::string& name);
}