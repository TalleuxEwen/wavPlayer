//
// Created by talleux on 2/28/24.
//

#include "AudioFile.hpp"

AudioFile::AudioFile(const std::shared_ptr<ArgumentsHandling>& argumentsHandling)
{
    _header = std::make_shared<Header>();
    _argumentsHandling = argumentsHandling;
    for (int i = 0; i < 4; i++)
    {
        _header->FileTypeBlockID += argumentsHandling->getOneByteFromFile()[0];
    }
    std::cout << "FileTypeBlockID: " << _header->FileTypeBlockID << std::endl;

    std::vector<char> FileSize = argumentsHandling->get4BytesFromFile();
    for (int i = 0; i < 4; i++)
    {
        _header->FileSize += (unsigned char)FileSize[i] << (8 * i);
    }
    std::cout << "fileSize: " << _header->FileSize << std::endl;

    for (int i = 0; i < 4; i++)
    {
        _header->FileFormatID += argumentsHandling->getOneByteFromFile()[0];
    }
    std::cout << "FileFormatID: " << _header->FileFormatID << std::endl;

    for (int i = 0; i < 4; i++)
    {
        _header->FormatBlockID += argumentsHandling->getOneByteFromFile()[0];
    }
    std::cout << "FormatBlockID: " << _header->FormatBlockID << std::endl;

    std::vector<char> FormatBlockSize = argumentsHandling->get4BytesFromFile();
    for (int i = 0; i < 4; i++)
    {
        _header->FormatBlockSize += (unsigned char)FormatBlockSize[i] << (8 * i);
    }
    std::cout << "FormatBlockSize: " << _header->FormatBlockSize << std::endl;

    std::vector<char> AudioFormat = argumentsHandling->get2BytesFromFile();
    for (int i = 0; i < 2; i++)
    {
        _header->AudioFormat += (unsigned char)AudioFormat[i] << (8 * i);
    }
    std::cout << "AudioFormat: " << _header->AudioFormat << std::endl;

    std::vector<char> NumChannels = argumentsHandling->get2BytesFromFile();
    for (int i = 0; i < 2; i++)
    {
        _header->NumChannels += (unsigned char)NumChannels[i] << (8 * i);
    }
    std::cout << "NumChannels: " << _header->NumChannels << std::endl;

    std::vector<char> SampleRate = argumentsHandling->get4BytesFromFile();
    for (int i = 0; i < 4; i++)
    {
        _header->SampleRate += (unsigned char)SampleRate[i] << (8 * i);
    }
    std::cout << "SampleRate: " << _header->SampleRate << std::endl;

    std::vector<char> BytePerSec = argumentsHandling->get4BytesFromFile();
    for (int i = 0; i < 4; i++)
    {
        _header->BytePerSec += (unsigned char)BytePerSec[i] << (8 * i);
    }
    std::cout << "BytePerSec: " << _header->BytePerSec << std::endl;

    std::vector<char> BytePerBlock = argumentsHandling->get2BytesFromFile();
    for (int i = 0; i < 2; i++)
    {
        _header->BytePerBlock += (unsigned char)BytePerBlock[i] << (8 * i);
    }
    std::cout << "BytePerBlock: " << _header->BytePerBlock << std::endl;

    std::vector<char> BitsPerSample = argumentsHandling->get2BytesFromFile();
    for (int i = 0; i < 2; i++)
    {
        _header->BitsPerSample += (unsigned char)BitsPerSample[i] << (8 * i);
    }
    std::cout << "BitsPerSample: " << _header->BitsPerSample << std::endl;

    std::string dataBlockID;
    while (dataBlockID != "data")
    {
        char byte = 0;
        while (byte != 'd')
        {
            dataBlockID = "";
            byte = argumentsHandling->getOneByteFromFile()[0];
            dataBlockID += byte;
        }
        for (int i = 0; i < 3; i++)
        {
            dataBlockID += argumentsHandling->getOneByteFromFile()[0];
        }
    }
    _header->DataBlockID = dataBlockID;
    std::cout << "DataBlockID: " << _header->DataBlockID << std::endl;

    std::vector<char> DataBlockSize = argumentsHandling->get4BytesFromFile();
    for (int i = 0; i < 4; i++)
    {
        _header->DataBlockSize += (unsigned char)DataBlockSize[i] << (8 * i);
    }
    std::cout << "DataBlockSize: " << _header->DataBlockSize << std::endl;

    //get the data
}

std::shared_ptr<Header> AudioFile::getHeader()
{
    return _header;
}

std::vector<short> AudioFile::getData()
{
    return _data;
}

void AudioFile::setData()
{
    std::cout << "Reading data" << std::endl;
    short data;
    short percent = 0;
    for (int i = 0; i < _header->DataBlockSize / 2; i++)
    {
        if (i * 100 / (_header->DataBlockSize / 2) > percent)
        {
            percent = i * 100 / (_header->DataBlockSize / 2);
            std::cout << "\033[1A";
            std::cout << "\r\033[K";
            std::cout << "Reading data: " << percent << "%" << std::endl;
        }
        std::vector<char> bytes = _argumentsHandling->get2BytesFromFile();
        data = 0;
        for (int j = 0; j < 2; j++)
        {
            data += (unsigned char)bytes[j] << (8 * j);
        }
        _data.push_back(data);
    }
    std::fflush(stdout);
    std::cout << "\033[2A";
    std::cout << "\r\033[K";
    std::cout << "File read" << std::endl;
    std::cout << "Preparing buffers" << std::endl;
    prepareBuffers();
    std::cout << "Buffers prepared" << std::endl;
}

void AudioFile::setChannels()
{
    for (int i = 0; i < _header->NumChannels; i++)
    {
        std::vector<short> channel;
        for (int j = 0; j < _header->DataBlockSize / 2; j += _header->NumChannels)
        {
            channel.push_back(_data[j + i]);
        }
        _channels.push_back(channel);
    }
    _data.clear();
}

void AudioFile::printChannel(short channel)
{
    int nbsamplesaverage = _channels[channel].size() / _channelPrintLength;

    for (int i = 0; i < _channelPrintHeight; i++)
    {
        for (int j = 0; j < _channelPrintLength; j++)
        {
            int average = 0;
            for (int k = 0; k < nbsamplesaverage; k++)
            {
                average += abs(_channels[channel][j * nbsamplesaverage + k]);
            }
            average /= nbsamplesaverage;
            average = average / 32767.0 * _channelPrintHeight;
            if (average >= _channelPrintHeight - i)
                std::cout << "*";
            else
                std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void AudioFile::prepareBuffers()
{
    _nbOfBuffers = _data.size() / 1024 + 1;
    _buffers = new short*[_nbOfBuffers];
    memset(_buffers, 0, _nbOfBuffers);
    for (int i = 0; i < _nbOfBuffers; i++)
    {
        _buffers[i] = new short[1024];
        for (int j = 0; j < 1024; j++)
        {
            if (i * 1024 + j < _data.size())
            {
                _buffers[i][j] = _data[i * 1024 + j];
            }
            else
            {
                _buffers[i][j] = 0;
            }
        }
    }
}

