cmake_minimum_required(VERSION 3.20)

project(MediaEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

add_executable(MediaEngineApp
        base/memory/noncopyable.h
        main.cc
        media/filters/audio_file_reader.cc
        media/filters/audio_file_reader.h
        media/filters/audio_file_writer.cc
        media/filters/audio_file_writer.h
        )
