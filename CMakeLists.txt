cmake_minimum_required(VERSION 3.16..3.22)

project(
        wavPlayer
        VERSION 0.1
        LANGUAGES CXX C)

add_executable(wavPlayer)

target_compile_features(wavPlayer PRIVATE cxx_std_17)
target_compile_options(wavPlayer PRIVATE ${CMAJ_WARNING_FLAGS})

target_sources(wavPlayer
        PRIVATE
        src/main.cpp
        src/AudioEngine/AudioEngine.cpp
        src/AudioEngine/AudioEngine.hpp
        src/ArgumentsHandling/ArgumentsHandling.cpp
        src/ArgumentsHandling/ArgumentsHandling.hpp
        src/AudioFile/AudioFile.cpp
        src/AudioFile/AudioFile.hpp
        src/AudioFile/Header.cpp
        src/AudioFile/Header.hpp
        )

find_package(RtAudio REQUIRED)
target_link_libraries(wavPlayer
        PRIVATE
        RtAudio::rtaudio
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,9.0>>:stdc++fs>
)
