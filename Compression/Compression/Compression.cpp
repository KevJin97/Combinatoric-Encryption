#include <iostream>

#include "CombinatoricCompress.hpp"
#include "BinaryConvert.hpp"
#include "BitPack.hpp"
#include "stringprocess.hpp"

std::vector<size_t> string2int(std::string& string);

int main()
{
    std::string string = "the quick brown fox jumped over the lazy dog";
    std::vector<size_t> data = string2int(string);

    size_t set_size = 65536;
    size_t size = 10;

    //TESTS
    //Compression combinatorics(set_size, size);
    //combinatorics.print();
    //combinatorics.sizecheck();
    
    uint128 num = 0;
    BitPack bitpack;
    for (;;)
    {
        BitPack bitpack = num;
        //bitpack.print();
        //++num;
    }
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