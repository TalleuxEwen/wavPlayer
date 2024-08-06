//
// Created by Ewen TALLEUX on 06/08/2024.
//

#include "AudioEngine/AudioEngine.hpp"
#include "AudioFile/AudioFile.hpp"
#include "AudioFile/Header.hpp"
#include "ArgumentsHandling/ArgumentsHandling.hpp"

int main(int argc, char *argv[])
{
    std::shared_ptr<AudioFile> audioFile = std::make_shared<AudioFile>(std::make_shared<ArgumentsHandling>(argc, argv));
    audioFile->setData();

    std::shared_ptr<AudioEngine> audioEngine = std::make_shared<AudioEngine>(audioFile);

    audioEngine->start();

    while (!audioEngine->isFinished())
    {
        // Wait for the audio to finish playing
    }

    return 0;
}
