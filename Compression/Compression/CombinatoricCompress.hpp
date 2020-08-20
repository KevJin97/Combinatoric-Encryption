#pragma once

#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include "BitPack.hpp"
typedef boost::multiprecision::uint128_t uint128;

template<typename T>
int compare(const void* a, const void* b)	//compare function for qsort
{
	if (*(T*)a < *(T*)b)
	{
		return -1;
	}
	else if (*(T*)a == *(T*)b)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


template<typename T>
struct Index
{
	T start;
	uint128 value;

	Index();
	Index(const Index& index);
	void operator=(const Index& index);
};

template<typename T>
Index<T>::Index()
{
	this->start = 0;
	this->value = 0;
}

template<typename T>
Index<T>::Index(const Index& index)
{
	this->start = index.start;
	this->value = index.value;
}

template<typename T>
void Index<T>::operator=(const Index& index)
{
	this->start = index.start;
	this->value = index.value;
}


template<typename T>
struct CompressedResult
{
	uint128 P_num;
	T C_start;
	uint128 C_num;

	CompressedResult() {}
	CompressedResult(const CompressedResult& result)
	{
		this->P_num = result.P_num;
		this->C_start = result.C_start;
		this->C_num = result.C_num;
	}
};


template<typename T>
class Compression
{
private:
	uint128** indices;
	size_t set_size;
	size_t size;
	//BitPack<T> bitpack;

	void set_indices(size_t set_size, size_t size);
	void delete_indices();

public:
	Compression();
	Compression(size_t set_size, size_t size);
	~Compression();

	uint128 factorial(uint128 n);
	uint128 C(size_t n, size_t r);
	uint128 C_r(size_t n, size_t r);
	uint128 P(size_t n, size_t r);
	uint128 P_r(size_t n, size_t r);
	T* combine(Index<T>& index);
	size_t* permute(uint128 index);
	Index<T> C_index(T* combinearray);
	uint128 P_index(size_t* permutearray);
	void set_value(size_t set_size, size_t size);
	CompressedResult<unsigned short> compress(T* inputarray);
	std::vector<T> decompress(CompressedResult<unsigned short>& compressed);
};

template<typename T>
Compression<T>::Compression()
{
	this->indices = NULL;
	this->set_size = 0;
	this->size = 0;
}

template<typename T>
Compression<T>::Compression(size_t set_size, size_t size)
{
	this->set_size = 0;
	this->size = 0;
	this->set_indices(set_size, size);
}

template<typename T>
Compression<T>::~Compression()
{
	this->delete_indices();
}

template<typename T>
uint128 Compression<T>::factorial(uint128 n)
{
	uint128 fact = 1;
	for (size_t i = 2; i <= n; i++)
	{
		fact *= i;
	}
	return fact;
}

template<typename T>
uint128 Compression<T>::C(size_t n, size_t r)
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

template<typename T>
uint128 Compression<T>::C_r(size_t n, size_t r)
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

template<typename T>
uint128 Compression<T>::P(size_t n, size_t r)
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

template<typename T>
uint128 Compression<T>::P_r(size_t n, size_t r)
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

template<typename T>
void Compression<T>::set_indices(size_t set_size, size_t size)
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

template<typename T>
void Compression<T>::delete_indices()
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

template<typename T>
T* Compression<T>::combine(Index<T>& index)
{
	T* value = new T[this->size];

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

template<typename T>
size_t* Compression<T>::permute(uint128 index)
{
	bool* p_used = new bool[this->size];
	for (size_t i = 0; i < this->size; i++)
	{
		p_used[i] = false;
	}

	uint128 increment = this->factorial(this->size - 1);
	size_t* value = new size_t[this->size];

	for (size_t loopindex = 0; loopindex < this->size; loopindex++)
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
				}

				break;
			}
		}
	}
	delete[] p_used;

	return value;
}

template<typename T>
Index<T> Compression<T>::C_index(T* combinearray)	//create indices vector and dot product with input arrazy might be faster
{
	Index<T> index;
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

template<typename T>
uint128 Compression<T>::P_index(size_t* permutearray)
{
	bool* p_used = new bool[size];
	for (size_t i = 0; i < size; i++)
	{
		p_used[i] = false;
	}

	uint128 increment = this->factorial(size - 1);
	uint128 index = 0;

	for (size_t loopindex = 0; loopindex < this->size; loopindex++)
	{
		for (size_t i = 0, j = 0; i < this->size; i++, j++)
		{
			while (p_used[j])
			{
				j++;
			}
			if (permutearray[loopindex] == j)
			{
				index += increment * i;
				p_used[j] = true;

				if (this->size != loopindex + 1)
				{
					increment /= this->size - loopindex - 1;
				}

				break;
			}
		}
	}

	return index;
}

template<typename T>
void Compression<T>::set_value(size_t set_size, size_t size)
{
	this->delete_indices();

	this->set_size = set_size;
	this->size = size;

	this->set_indices(set_size, size);
}

template<typename T>
CompressedResult<unsigned short> Compression<T>::compress(T* inputarray)
{
	CompressedResult<unsigned short> result;	//this will be returned

	T* sortedarray = new T[this->size];	//array that holds sorted list
	for (size_t i = 0; i < this->size; ++i)	//initialize sortedarray
	{
		sortedarray[i] = inputarray[i];
	}
	std::qsort(sortedarray, this->size, sizeof(T), compare<T>);	//quicksort

	Index<T> c_index = this->C_index(sortedarray);	//get combination index
	
	size_t* order = new size_t[this->size];	//holds permutation order
	
	bool* p_used = new bool[this->size];
	for (size_t i = 0; i < this->size; ++i)	//initialize p_used
	{
		p_used[i] = false;
	}

	for (size_t i = 0, j = 0; i < this->size; ++i, j = 0)	//set permutation array
	{
		while (j < this->size)
		{
			if (!p_used[j])
			{
				if (inputarray[i] == sortedarray[j])
				{
					p_used[j] = true;
					order[i] = j;
					break;
				}
				else
				{
					++j;
				}
			}
			else
			{
				++j;
			}
		}
	}

	uint128 p_index = this->P_index(order);	//get permutation index

	//set all outputs
	result.P_num = p_index;
	result.C_start = c_index.start;
	result.C_num = c_index.value;
	
	delete[] sortedarray;
	delete[] order;
	delete[] p_used;

	return result;
}

template<typename T>
std::vector<T> Compression<T>::decompress(CompressedResult<unsigned short>& compressed)
{
	std::vector<T> decompressed;	//this will be returned
	decompressed.resize(this->size);	//just making it more efficient

	uint128 p_index = compressed.P_num;
	
	Index<T> c_index;
	c_index.start = compressed.C_start;
	c_index.value = compressed.C_num;

	T* combinearray = this->combine(c_index);	//get original array components
	
	size_t* permutearray = this->permute(p_index);	//get original order
	
	for (size_t i = 0; i < this->size; ++i)	//reorder values
	{
		decompressed[i] = combinearray[permutearray[i]];
	}

	delete[] combinearray;
	delete[] permutearray;

	return decompressed;
}