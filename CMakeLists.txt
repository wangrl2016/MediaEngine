cmake_minimum_required(VERSION 3.20)

project(MediaEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

add_executable(MediaEngineApp
        base/memory/noncopyable.h
        base/time/time_delta.cc
        base/time/time_delta.h
        main.cc
        media/base/audio_parameters.cc
        media/base/audio_parameters.h
        media/filters/audio_file_reader.cc
        media/filters/audio_file_reader.h
        media/filters/audio_file_writer.cc
        media/filters/audio_file_writer.h
        media/filters/ffmpeg_glue.cc
        media/filters/ffmpeg_glue.h
        media/filters/ffmpeg_url_protocol.h
        media/filters/memory_url_protocol.cc
        media/filters/memory_url_protocol.h
        )

target_link_libraries(MediaEngineApp
        ${CONAN_LIBS}
        )
