//
// Created by talleux on 2/28/24.
//

#pragma once

#include <memory>
#include "../ArgumentsHandling/ArgumentsHandling.hpp"
#include "Header.hpp"

class AudioFile
{
    public:
        explicit AudioFile(const std::shared_ptr<ArgumentsHandling>& argumentsHandling);
        ~AudioFile() = default;

        std::shared_ptr<Header> getHeader();

        std::vector<short> getData();

        void setData();

        void setChannels();

        void printChannel(short channel);

        void printChannels();

        short getFirstValueInDataAndRemoveIt();

        void prepareBuffers();

        int _nbOfBuffers = 0;
        int _nbReadBuffers = 0;

        short **getBuffers()
        {
            return _buffers;
        }

    private:
        std::shared_ptr<Header> _header;
        std::vector<short> _data;
        std::shared_ptr<ArgumentsHandling> _argumentsHandling;
        std::vector<std::vector<short>> _channels;
        short _channelPrintLength = 500;
        short _channelPrintHeight = 20;
        short **_buffers;
};
