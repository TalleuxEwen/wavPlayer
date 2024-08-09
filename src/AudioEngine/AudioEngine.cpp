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

void printingThread(AudioEngine *engine)
{
    std::cout << displayTime(0) << " / " << displayTime((engine->_audioFile->getHeader()->DataBlockSize / 4) / engine->sampleRate) << std::endl;
    engine->printWaveform(engine->_audioFile->getBuffers(), engine->bufferFrames, 0, engine->_audioFile->_nbOfBuffers);

    while (!engine->isFinished()) {
        std::cout << "\r\033[22A" << displayTime(engine->_audioFile->_nbReadBuffers * engine->bufferFrames / engine->sampleRate) << " / " << displayTime((engine->_audioFile->getHeader()->DataBlockSize / 4) / engine->sampleRate) << std::endl;
        std::cout << std::flush;
        engine->printWaveform(engine->_audioFile->getBuffers(), engine->bufferFrames, engine->_audioFile->_nbReadBuffers, engine->_audioFile->_nbOfBuffers);
        std::cout << std::flush;
    }
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
    pthread_join(_thread, nullptr);
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
        pthread_create(&_thread, nullptr, (void *(*)(void *))printingThread, this);
    } catch (...) {
        std::cout << "Error starting audio stream" << std::endl;
        exit(1);
    }
}

void AudioEngine::printWaveform(short **buffers, unsigned int nBufferFrames, int index, int nbTotalBuffers) {
    char **waveform = new char *[21];
    for (int i = 0; i < 21; i++) {
        waveform[i] = new char[41];
        for (int j = 0; j < 41; j++) {
            waveform[i][j] = ' ';
        }
    }
    int toFill = 20 - index;
    int toFillEnd = nbTotalBuffers - index;

    if (toFill < 0) {
        toFill = 0;
    }
    if (toFillEnd < 20) {
        toFillEnd = 20;
    }
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 41; j++) {
            if (j < toFill) {
                waveform[i][j] = ' ';
            } else if (j > toFillEnd) {
                waveform[i][j] = ' ';
            } else
            {
                int valueLeft = normalizeBetween0And10(getMaxValue(buffers[index + (j - 20)], nBufferFrames, true));
            int valueRight = normalizeBetween0And10(getMaxValue(buffers[index + (j - 20)], nBufferFrames, false));
                if (i < 10) {
                    waveform[i][j] = valueLeft >= 10 - i ? '*' : ' ';
                }
                if (i > 10) {
                    waveform[i][j] = valueRight >= i - 10 ? '*' : ' ';
                }

            }
            if (j == 20) {
                waveform[i][j] = '|';
            }
            if (i == 10) {
                waveform[i][j] = '-';
            }
        }
    }
    std::ostringstream color;
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 41; j++) {
            if (waveform[i][j] == '*') {
                if (i < 10) {
                    color << _colors[i];
                    std::cout << color.str() << "█" << "\033[0m";
                } else if (i > 10) {
                    color << _colors[i - 1];
                    std::cout << color.str() << "█" << "\033[0m";
                }
            } else {
                std::cout << waveform[i][j];
            }
        }
        std::cout << std::endl;
    }

    //free memory
    for (int i = 0; i < 21; i++) {
        delete[] waveform[i];
    }
    delete[] waveform;
}

int AudioEngine::getMaxValue(short *buffer, unsigned int nBufferFrames, bool left) {
    int max = 0;
    if (buffer == nullptr) {
        return 0;
    }
    for (unsigned int i = 0; i < nBufferFrames; i++) {
        if (left) {
            if (i % 2 == 0) {
                if (buffer[i] > max) {
                    max = buffer[i];
                }
            }
        } else {
            if (i % 2 != 0) {
                if (buffer[i] > max) {
                    max = buffer[i];
                }
            }
        }
    }
    return max;
}

int AudioEngine::normalizeBetween0And10(int value)
{
    return value / 32767.0 * 10;
}
