#include <iostream>

#include "CombinatoricCompress.hpp"
#include "stringprocess.hpp"

std::vector<size_t> string2int(std::string& string);

int main()
{
    std::string string = "the quick brown fox jumped over the lazy dog";
    std::vector<size_t> data = string2int(string);

    size_t set_size = 256;
    size_t size = 10;

    size_t* p_array;
    Index index;
    Compression combinatorics(set_size, size);

    //TEST CODE
    combinatorics.print();

    /*
    for (size_t i = 0; i < set_size; ++i)   //prints index with accompanying vector
    {
        index.start = i;
        Number startindex = combinatorics.return_index_size(i);

        for (Number j = 0; j < startindex; ++j)
        {
            index.value = j;
            p_array = combinatorics.combine(index);

            std::cout << "Index: ";
            j.print();
            for (size_t k = 0; k < size; k++)
            {
                std::cout << p_array[k] << " ";
            }   std::cout << " " << std::endl;

            delete[] p_array;
        }
    }
    */
    
    
    //testing largest index size
    for (size_t val = 0; val < set_size; val++)
    {
        p_array = new size_t[size];
        p_array[0] = val;
        for (size_t i = 1; i < size; i++)
        {
            p_array[i] = set_size - 1;
            //p_array[i] = val;
        }
        index = combinatorics.C_index(p_array);
        std::cout << "Index: " << index.start << std::endl;
        index.value.print();
        delete[] p_array;
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