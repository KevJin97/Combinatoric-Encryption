#pragma once

#include <boost/multiprecision/cpp_int.hpp>
typedef boost::multiprecision::uint128_t uint128;

struct Index
{
	size_t start;
	uint128 value;

	Index();
	Index(const Index& index);
	void operator=(const Index& index);
};

Index::Index()
{
	this->start = 0;
	this->value = 0;
}

Index::Index(const Index& index)
{
	this->start = index.start;
	this->value = index.value;
}

void Index::operator=(const Index& index)
{
	this->start = index.start;
	this->value = index.value;
}


class Compression
{
private:
	uint128** indices;
	size_t set_size;
	size_t size;

public:
	Compression();
	Compression(size_t set_size, size_t size);
	~Compression();

	uint128 factorial(uint128 n);
	uint128 C(size_t n, size_t r);
	uint128 C_r(size_t n, size_t r);
	uint128 P(size_t n, size_t r);
	uint128 P_r(size_t n, size_t r);
	void set_indices(size_t set_size, size_t size);
	void delete_indices();
	size_t* combine(Index index);
	size_t* permute(uint128 index);
	Index C_index(size_t* combinearray);
	uint128 P_index(size_t* permutearray);
	void set_value(size_t set_size, size_t size);
	//compress()
	//decompress()
	void print()
	{
		for (size_t i = 0; i < this->size; i++)
		{
			std::cout << "Number " << i << ": " << std::endl;

			for (size_t j = 0; j < this->set_size; j++)
			{
				std::cout << j << ":\t";
				std::cout << std::hex << this->indices[i][j] << std::endl;
			}
		}
	}
	void sizecheck()
	{
		uint128 MAX_128 = 0 - 1;
		size_t MAX_64 = 0 - 1;
		//std::cout << "Max Value: " << MAX_128 << std::endl;

		MAX_128 = this->factorial(this->size - 1) - 1;
		std::cout << "Permutation: " << MAX_128 << "\t";
		if (MAX_128 <= MAX_64)
		{
			std::cout << "Can hold with 8 bytes" << "\n" << std::endl;
		}
		else
		{
			std::cout << "ERROR" << "\n" << std::endl;
		}


		//MAX_128 = MAX_64;
		//++MAX_128;

		std::cout << "Largest C for each start point" << std::endl;

		size_t* p_array = new size_t[this->size];
		Index index;

		for (size_t j = 0; j < this->size; j++)
		{
			p_array[j] = this->set_size - 1;
		}

		for (size_t i = 0; i < this->set_size; i++)
		{
			std::cout << "Start: " << i << std::endl;
			p_array[0] = i;

			index = this->C_index(p_array);
			//uint128 div = index.value / MAX_128;
			//uint128 mod = index.value % MAX_128;
			std::cout << "Value:\t" << index.value << std::endl;
			std::cout << std::endl;
			//std::cout << "Divide_64:\t" << div << std::endl;
			//std::cout << "Mod_64:\t\t" << mod << std::endl;

			/*
			if (index.value <= MAX_64)
			{
				std::cout << "Can hold with 8 bytes" << "\n" << std::endl;
			}
			else
			{
				std::cout << "ERROR" << "\n" << std::endl;
			}
			*/
		}

		delete[] p_array;
	}
};

Compression::Compression()
{
	this->indices = NULL;
	this->set_size = 0;
	this->size = 0;
}

Compression::Compression(size_t set_size, size_t size)
{
	this->set_size = 0;
	this->size = 0;
	this->set_indices(set_size, size);
}

Compression::~Compression()
{
	this->delete_indices();
}

uint128 Compression::factorial(uint128 n)
{
	uint128 fact = 1;
	for (size_t i = 2; i <= n; i++)
	{
		fact *= i;
	}
	return fact;
}

uint128 Compression::C(size_t n, size_t r)
{
	uint128 c = 0;

	if (n >= r)
	{
		uint128* prev;
		uint128* p_pascal = new uint128[2];
		p_pascal[0] = 1;
		p_pascal[1] = 1;

		for (size_t i = 2; i <= n; i++)
		{
			prev = p_pascal;

			p_pascal = new uint128[i + 1];
			p_pascal[0] = 1;
			p_pascal[i] = 1;

			for (size_t j = 1; j < i; j++)
			{
				p_pascal[j] = prev[j - 1] + prev[j];
			}

			delete[] prev;
		}

		c = p_pascal[r];
		delete[] p_pascal;
	}

	return c;
}

uint128 Compression::C_r(size_t n, size_t r)
{
	if (r == 0)
	{
		return 0;
	}
	else
	{
		n += r - 1;

		return this->C(n, r);
	}
}

uint128 Compression::P(size_t n, size_t r)
{
	uint128 p = 1;

	if (n >= r)
	{
		for (size_t m = n - r; n > m; n--)
		{
			p *= n;
		}
	}

	return p;
}

uint128 Compression::P_r(size_t n, size_t r)
{
	uint128 pr = 1;
	if (n >= r)
	{
		for (size_t i = 0; i < r; i++)
		{
			pr *= n;
		}
	}

	return pr;
}

void Compression::set_indices(size_t set_size, size_t size)
{
	this->delete_indices();
	this->set_size = set_size;
	this->size = size;

	this->indices = new uint128*[size];

	this->indices[size - 1] = new uint128[set_size];
	for (size_t i = 0; i < set_size; i++)
	{
		this->indices[size - 1][i] = 1;
	}
	for (size_t i = 1, j = size - 2; i < size; i++, j--)
	{
		this->indices[j] = new uint128[set_size];
		this->indices[j][set_size - 1] = 1;

		for (size_t k = 1, l = set_size - 1; k < set_size; k++, l--)
		{
			this->indices[j][l - 1] = this->indices[j + 1][l - 1] + this->indices[j][l];
		}
	}
}

void Compression::delete_indices()
{
	if (this->size != 0)
	{
		for (size_t i = 0; i < this->size; i++)	//delete indices
		{
			delete[] this->indices[i];
		}
		delete[] indices;
	}
}

size_t* Compression::combine(Index index)
{
	size_t* value = new size_t[this->size];

	size_t indexstart = index.start;
	uint128 valuestart = 0;

	value[0] = index.start;

	for (size_t loopindex = 1, loopcount = this->size; loopindex < loopcount; loopindex++)
	{
		for (size_t i = indexstart; i < this->set_size; i++)
		{
			uint128 j = this->indices[loopindex][i];

			if (index.value < valuestart + j)
			{
				value[loopindex] = i;
				break;
			}
			else
			{
				valuestart += j;
				indexstart++;
			}
		}
	}

	return value;
}

size_t* Compression::permute(uint128 index)
{
	bool* p_used = new bool[this->size];
	for (size_t i = 0; i < this->size; i++)
	{
		p_used[i] = false;
	}

	uint128 increment = this->factorial(this->size - 1);
	size_t* value = new size_t[this->size];

	for (size_t loopindex = 0, loopcount = this->size; loopindex < loopcount; loopindex++)
	{
		for (size_t j = 0, i = 0; i < this->size; i++, j++)
		{
			while (p_used[j])
			{
				j++;
			}
			if (index < increment * (i + 1))
			{
				index -= increment * i;

				value[loopindex] = j;	//sets value
				p_used[j] = true;

				if (this->size != loopindex + 1)
				{
					increment /= this->size - loopindex - 1;
				};

				break;
			}
		}
	}
	delete[] p_used;

	return value;
}

Index Compression::C_index(size_t* combinearray)	//create indices vector and dot product with input arrazy might be faster
{
	Index index;
	index.start = combinearray[0];

	size_t indexstart = index.start;
	uint128 valuestart = 0;

	for (size_t loopindex = 1, loopcount = this->size; loopindex < loopcount; loopindex++)
	{
		for (size_t i = indexstart; i < this->set_size; i++)
		{
			uint128 j = this->indices[loopindex][i];

			if (combinearray[loopindex] == i)
			{
				index.value = valuestart;
				break;
			}
			else
			{
				indexstart++;
				valuestart += j;
			}
		}
	}

	return index;
}

uint128 Compression::P_index(size_t* permutearray)
{
	size_t size = this->size;

	bool* p_used = new bool[this->size];
	for (size_t i = 0; i < this->size; i++)
	{
		p_used[i] = false;
	}

	uint128 fact = this->factorial(size);
	uint128 increment = fact / size;
	uint128 index = 0;

	for (size_t loopindex = 0, loopend = size; loopindex < loopend; loopindex++)
	{
		for (size_t i = 0, j = 0; i < size; i++, j++)
		{
			while (p_used[j])
			{
				j++;
			}
			if (permutearray[loopindex] == j)
			{
				index += increment * i;
				increment--;
				size--;
				p_used[j] = true;

				if (size != loopindex + 1)
				{
					increment /= size - loopindex - 1;
				}

				break;
			}
		}
	}

	return index;
}

void Compression::set_value(size_t set_size, size_t size)
{
	this->delete_indices();

	this->set_size = set_size;
	this->size = size;

	this->set_indices(set_size, size);
}