//
// Created by ewen.talleux on 27/02/2024.
//

#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class ArgumentsHandling
{
    public:
        ArgumentsHandling(int argc, char *argv[]);

        std::vector<char> getOneByteFromFile();
        std::vector<char> get2BytesFromFile();
        std::vector<char> get4BytesFromFile();

        std::vector<char> putInLittleEndian(std::vector<char> bytes);

    private:
        std::ifstream _file;

};
