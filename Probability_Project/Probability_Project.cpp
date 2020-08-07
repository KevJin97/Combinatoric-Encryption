#include <iostream>
#include "probability.hpp"

int main()
{	
	for (size_t i = 0; i < 20; i++)
	{
		size_t* p = Combination::combine(4, 3, i);
		size_t index = Combination::C_index(4, 3, p);
		std::cout << index << std::endl;
		delete[] p;
	}

	return 0;
}