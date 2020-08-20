#include <iostream>
#include "CombinatoricCompress.hpp"
#include "stringprocess.hpp"
#include "probability.hpp"

int main()
{
    size_t set_size = 65536;
    size_t size = 10;

    Probability prob;
    unsigned short test[10];
    for (size_t i = 0; i < 10; ++i)
    {
        test[i] = prob.random_gen(0, set_size);
    }

    Compression<unsigned short> combinatorics(set_size, size);
    
    CompressedResult<unsigned short> result = combinatorics.compress(test);
    std::cout << "P Num:\t " << result.P_num << std::endl;
    std::cout << "C Start: " << result.C_start << std::endl;
    std::cout << "C Num:\t" << result.C_num << std::endl;

    std::vector<unsigned short> decomp = combinatorics.decompress(result);
    std::cout << "\nDecompressed" << std::endl;
    for (size_t i = 0; i < decomp.size(); ++i)
    {
        std::cout << decomp[i] << "\t" << test[i] << std::endl;;
    }

    return 0;
}