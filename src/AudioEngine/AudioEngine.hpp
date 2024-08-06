//
// Created by Ewen TALLEUX on 06/08/2024.
//

#pragma once

#include <RtAudio.h>
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

    private:
        #ifdef __linux__
            RtAudio adc = RtAudio(RtAudio::Api::LINUX_PULSE);
        #else
            RtAudio adc = RtAudio(RtAudio::Api::MACOSX_CORE);
        #endif

        RtAudio::StreamParameters outputParams;

        unsigned int sampleRate;
        unsigned int bufferFrames = 512;

        std::shared_ptr<AudioFile> _audioFile;

        bool _finished = false;
};
