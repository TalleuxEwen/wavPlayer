//
// Created by ewen.talleux on 27/02/2024.
//

#include <vector>
#include "ArgumentsHandling.hpp"

ArgumentsHandling::ArgumentsHandling(int argc, char *argv[])
{
    //trying to open a file
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        exit(1);
    }
    _file = std::ifstream(argv[1]);
    if (!_file.is_open())
    {
        std::cerr << "Could not open file " << argv[1] << std::endl;
        exit(1);
    }
}

std::vector<char> ArgumentsHandling::getOneByteFromFile()
{
    std::vector<char> bytes;

    char byte;
    _file.read(&byte, 1);
    if (_file.eof())
    {
        std::cerr << "End of file reached" << std::endl;
        exit(1);
    }
    bytes.push_back(byte);
    return bytes;
}

std::vector<char> ArgumentsHandling::get2BytesFromFile()
{
    std::vector<char> bytes;

    char byte;
    for (int i = 0; i < 2; i++)
    {
        _file.read(&byte, 1);
        if (_file.eof())
        {
            std::cerr << "End of file reached" << std::endl;
            exit(1);
        }
        bytes.push_back(byte);
    }
    return bytes;
}

std::vector<char> ArgumentsHandling::get4BytesFromFile()
{
    std::vector<char> bytes;

    char byte;
    for (int i = 0; i < 4; i++)
    {
        _file.read(&byte, 1);
        if (_file.eof())
        {
            std::cerr << "End of file reached" << std::endl;
            exit(1);
        }
        bytes.push_back(byte);
    }
    return bytes;
}

std::vector<char> ArgumentsHandling::putInLittleEndian(std::vector<char> bytes)
{
    std::vector<char> littleEndian;
    littleEndian.reserve(bytes.size());
    for (int i = 0; i < bytes.size(); i++)
    {
        littleEndian.push_back(bytes[bytes.size() - i - 1]);
    }
    return littleEndian;
}
