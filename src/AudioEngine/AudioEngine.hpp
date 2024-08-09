//
// Created by Ewen TALLEUX on 06/08/2024.
//

#pragma once

#include <RtAudio.h>
#include <pthread.h>
#include "../AudioFile/AudioFile.hpp"

class AudioEngine {
    public:
        AudioEngine(std::shared_ptr<AudioFile> audioFile);
        ~AudioEngine();

        void stop();

        static int callback(void* outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData);

        void start();

        bool isFinished() const {
            return _finished;
        }


        float transformShortToFloat(short value) {
            return (float)value / 32768.0f;
        }

        void printWaveform(short **buffers, unsigned int nBufferFrames, int index, int nbTotalBuffers);

        int getMaxValue(short *buffer, unsigned int nBufferFrames, bool stereo);

        int normalizeBetween0And10(int value);

        std::shared_ptr<AudioFile> _audioFile;

        unsigned int sampleRate;
        unsigned int bufferFrames = 512;

    private:
        #ifdef __linux__
            RtAudio adc = RtAudio(RtAudio::Api::LINUX_PULSE);
        #else
            RtAudio adc = RtAudio(RtAudio::Api::MACOSX_CORE);
        #endif

        RtAudio::StreamParameters outputParams;

        bool _finished = false;

        pthread_t _thread;

        char **_colors = new char*[20] {
            (char *)"\033[38;2;0;0;255m",
            (char *)"\033[38;2;28;0;226m",
            (char *)"\033[38;2;56;0;198m",
            (char *)"\033[38;2;85;0;170m",
            (char *)"\033[38;2;113;0;141m",
            (char *)"\033[38;2;141;0;113m",
            (char *)"\033[38;2;170;0;85m",
            (char *)"\033[38;2;198;0;56m",
            (char *)"\033[38;2;226;0;28m",
            (char *)"\033[38;2;255;0;0m",
            (char *)"\033[38;2;255;0;0m",
            (char *)"\033[38;2;226;0;28m",
            (char *)"\033[38;2;198;0;56m",
            (char *)"\033[38;2;170;0;85m",
            (char *)"\033[38;2;141;0;113m",
            (char *)"\033[38;2;113;0;141m",
            (char *)"\033[38;2;85;0;170m",
            (char *)"\033[38;2;56;0;198m",
            (char *)"\033[38;2;28;0;226m",
            (char *)"\033[38;2;0;0;255m"
        };
};
