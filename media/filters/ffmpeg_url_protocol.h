//
// Created by wangrl2016 on 2023/3/29.
//

#pragma once

#include <cstdint>

namespace media {
    class FFmpegURLProtocol {
    public:
        // Read the given of bytes into data, returns the number of bytes read if successful.
        virtual int Read(int size, uint8_t* data) = 0;

        // Returns true and the current file position for this file, false if the
        // file position could not be retrieved.
        virtual bool GetPosition(int64_t* position_out) = 0;

        // Returns true if the file position could be set, false otherwise.
        virtual bool SetPosition(int64_t position) = 0;

        // Returns true and the file size, false if the file size could not be retrieved.
        virtual bool GetSize(int64_t* size_out) = 0;
    };
}