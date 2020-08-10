#include <iostream>

#include "CombinatoricCompress.hpp"
#include "stringprocess.hpp"

std::vector<size_t> string2int(std::string& string);

int main()
{
    std::string string = "the quick brown fox jumped over the lazy dog";
    std::vector<size_t> data = string2int(string);

    unsigned int MaxInt = 0 - 1;
    size_t set_size = ++MaxInt;
    size_t size = 10;

    Compression combinatorics(set_size, size);

    return 0;
}

std::vector<size_t> string2int(std::string& string)
{
    std::vector<size_t> intvec;
    intvec.resize(string.size());

    for (size_t i = 0; i < string.size(); i++)
    {
        intvec[i] = string[i];
    }

    return intvec;
}