//
// Created by Ewen TALLEUX on 06/08/2024.
//

#include "AudioEngine.hpp"

std::string displayTime(int seconds)
{
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;

    //every part must be two digits long ex : 01:02:03

    std::string offsethour = hours < 10 ? "0" : "";
    std::string offsetminute = minutes < 10 ? "0" : "";
    std::string offsetsecond = seconds < 10 ? "0" : "";

    std::string result = offsethour + std::to_string(hours) + ":" + offsetminute + std::to_string(minutes) + ":" + offsetsecond + std::to_string(seconds);
    return result;
}

AudioEngine::AudioEngine(std::shared_ptr<AudioFile> audioFile) : _audioFile(std::move(audioFile))
{
    std::cout << "Audio API used: " << adc.getCurrentApi() << std::endl;

    outputParams.deviceId = adc.getDefaultOutputDevice();
    outputParams.nChannels = adc.getDeviceInfo(outputParams.deviceId).outputChannels;
    outputParams.firstChannel = 0;

    sampleRate = adc.getDeviceInfo(outputParams.deviceId).preferredSampleRate;
}

AudioEngine::~AudioEngine() {
    stop();
    if (adc.isStreamOpen()) {
        adc.closeStream();
    }
}

void AudioEngine::stop() {
    adc.stopStream();
}

int AudioEngine::callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData)
{
    short *result;

    AudioEngine *engine = (AudioEngine *)userData;

    if (engine->_audioFile->_nbReadBuffers == engine->_audioFile->_nbOfBuffers) {
        engine->_finished = true;
        return 1;
    }

    result = engine->_audioFile->getBuffers()[engine->_audioFile->_nbReadBuffers];
    engine->_audioFile->_nbReadBuffers++;

    for (unsigned int i = 0; i < nBufferFrames * 2; i++) {
        ((short *)outputBuffer)[i] = result[i];
    }
    std::cout << "\r\033[1A" << displayTime(engine->_audioFile->_nbReadBuffers * nBufferFrames / engine->sampleRate) << " / " << displayTime((engine->_audioFile->getHeader()->DataBlockSize / 4) / engine->sampleRate) << std::endl;
    std::cout << std::flush;
    return 0;
}

void AudioEngine::start() {
    try {
        RtAudio::StreamOptions options;
        options.streamName = "Player";
        sampleRate = _audioFile->getHeader()->SampleRate;
        std::cout << "Sample rate: " << sampleRate << std::endl;
        adc.openStream(&outputParams, nullptr, RTAUDIO_SINT16, sampleRate, &bufferFrames, &callback, this, &options);
        adc.startStream();
        std::cout << "The file last " << (_audioFile->getHeader()->DataBlockSize / 4) / sampleRate << " seconds" << std::endl;
        std::cout << displayTime(0) << " / " << displayTime((_audioFile->getHeader()->DataBlockSize / 4) / sampleRate) << std::endl;
    } catch (...) {
        std::cout << "Error starting audio stream" << std::endl;
        exit(1);
    }
}

