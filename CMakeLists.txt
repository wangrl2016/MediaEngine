cmake_minimum_required(VERSION 3.20)

project(MediaEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

set(BASE_SOURCES
        base/memory/noncopyable.h
        base/time/time_base.h
        base/time/time_delta.cc
        base/time/time_delta.h
        )

set(MEDIA_SOURCES
        media/base/audio_parameters.cc
        media/base/audio_parameters.h
        media/base/decoder_buffer.cc
        media/base/decoder_buffer.h
        media/ffmpeg/ffmpeg_common.cc
        media/ffmpeg/ffmpeg_common.h
        media/ffmpeg/ffmpeg_delete.h
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

add_library(MediaEngineLib STATIC)
target_sources(MediaEngineLib
        PRIVATE
        ${BASE_SOURCES}
        ${MEDIA_SOURCES}
        )

add_executable(MediaEngineApp
        main.cc
        )

target_link_libraries(MediaEngineApp
        ${CONAN_LIBS}
        MediaEngineLib
        )

# build tests
enable_testing()

add_executable(MediaEngineTest
        media/filters/audio_file_reader_test.cc
        )

target_link_libraries(MediaEngineTest
        ${CONAN_LIBS}
        MediaEngineLib)

include(GoogleTest)
gtest_discover_tests(MediaEngineTest)

