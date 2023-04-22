//
// Created by wangrl2016 on 2023/4/22.
//

#include <filesystem>
#include <fstream>
#include <string>
#include "media/base/test_data_util.h"

namespace media {
    std::string GetTestDataFilePath(const std::string& name) {
        return kTestDataPath + name;
    }

    std::shared_ptr<DecoderBuffer> ReadTestDataFile(const std::string& name) {
        std::string file_path = GetTestDataFilePath(name);
        auto file_size = std::filesystem::file_size(file_path);
        std::shared_ptr<DecoderBuffer> buffer(new DecoderBuffer(file_size));
        auto* data = reinterpret_cast<char*>(buffer->writable_data());
        std::ifstream ifs(file_path);
        ifs.read(data, long(file_size));
        ifs.close();
        return buffer;
    }
}