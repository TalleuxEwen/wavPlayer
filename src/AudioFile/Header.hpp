//
// Created by talleux on 2/28/24.
//

#pragma once


#include <string>

class Header
{
    public:
        std::string FileTypeBlockID;
        long FileSize;
        std::string FileFormatID;
        std::string FormatBlockID;
        long FormatBlockSize;
        short AudioFormat;
        short NumChannels;
        long SampleRate;
        long BytePerSec;
        short BytePerBlock;
        short BitsPerSample;
        std::string DataBlockID;
        long DataBlockSize;
};
