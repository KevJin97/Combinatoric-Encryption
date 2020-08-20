#pragma once

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <bitset>
#include <boost/multiprecision/cpp_int.hpp>
typedef boost::multiprecision::uint128_t uint128;

const uint128 MAX_8 = 18446744073709551615;
const uint128 MAX_4 = 4294967295;
const uint128 MAX_2 = 65535;
const uint128 MAX_1 = 255;

class Binary
{
protected:
	size_t size;
	bool* bits;

public:
	Binary();
	Binary(const Binary& cpy);
	Binary(std::vector<Binary>& b_list);
	Binary(bool* bitlist, size_t size);
	Binary(uint128 number, size_t size);
	Binary(unsigned char number);
	Binary(unsigned short number);
	Binary(unsigned int number);
	Binary(unsigned long long number);
	Binary(uint128 number);
	~Binary();

	void set(bool* bitlist, size_t size);
	std::vector<Binary> split(size_t num);	//does not have a safety
	void resize(size_t size);
	void merge(Binary& bin);
	void clear();
	void print();
	bool* return_bits();
	size_t return_size();
	uint128 return_value();

	void operator=(Binary& binary);
	void operator=(std::vector<Binary>& b_list);
	void operator=(unsigned char num);		//8 bit
	void operator=(unsigned short num);		//16 bit
	void operator=(unsigned int num);		//32 bit
	void operator=(unsigned long long num);	//64 bit
	void operator=(uint128& num);			//128 bit
	bool operator[](size_t index);
	Binary& operator++();
	Binary operator++(int);
};

Binary::Binary()
{
	this->size = 0;
	this->bits = NULL;
}

Binary::Binary(bool* bitlist, size_t size)
{
	this->bits = new bool[size];
	this->size = size;

	for (size_t i = 0; i < size; i++)
	{
		this->bits[i] = bitlist[i];
	}
}

Binary::Binary(uint128 number, size_t size)
{
	*this = number;
	this->resize(size);
}

Binary::Binary(const Binary& cpy)
{
	this->size = cpy.size;
	this->bits = new bool[cpy.size];
	
	for (size_t i = 0; i < cpy.size; i++)
	{
		this->bits[i] = cpy.bits[i];
	}
}

Binary::Binary(std::vector<Binary>& b_list)
{
	*this = b_list;
}

Binary::Binary(unsigned char number)
{
	*this = number;
}

Binary::Binary(unsigned short number)
{
	*this = number;
}

Binary::Binary(unsigned int number)
{
	*this = number;
}

Binary::Binary(unsigned long long number)
{
	*this = number;
}

Binary::Binary(uint128 number)
{
	*this = number;
}

Binary::~Binary()
{
	this->clear();
}

void Binary::set(bool* bitlist, size_t size)
{
	this->clear();

	this->bits = new bool[size];
	this->size = size;

	for (size_t i = 0; i < size; i++)
	{
		this->bits[i] = bitlist[i];
	}
}

std::vector<Binary> Binary::split(size_t num)
{
	std::vector<Binary> binaries;

	size_t newsize = this->size / num;
	if (newsize >= 8)
	{
		binaries.resize(num);
		bool* b_array = new bool[newsize];

		for (size_t index = 0, start = 0, bin_index = 0; index < this->size; ++bin_index)
		{
			start = 0;
			while (start < newsize)
			{
				b_array[start] = this->bits[index];
				++index;
				++start;
			}
			binaries[bin_index].set(b_array, newsize);
		}

		delete[] b_array;
	}
	else
	{
		binaries.push_back(*this);
	}
	return binaries;
}

void Binary::resize(size_t size)
{
	bool* hold = this->bits;
	if (this->size > size)
	{
		this->bits = new bool[size];
		for (size_t i = this->size - size, j = 0; i < this->size; ++i, ++j)	//remove starting from MSB
		{
			this->bits[j] = hold[i];
		}
		this->size = size;
		delete[] hold;
	}
	else if (this->size < size)
	{
		this->bits = new bool[size];
		
		size_t index = 0;
		while (index < size - this->size)	//add zeros
		{
			this->bits[index] = false;
			++index;
		}
		for (size_t i = 0; i < this->size; ++i, ++index)
		{
			this->bits[index] = hold[i];
		}
		this->size = size;
		delete[] hold;
	}
}

void Binary::merge(Binary& bin)
{
	if (this->size != 0)
	{
		bool* merged = new bool[this->size + bin.size];

		size_t newindex = 0;
		size_t compindex = 0;
		while (compindex < this->size)
		{
			merged[newindex] = this->bits[compindex];
			++newindex;
			++compindex;
		}
		compindex = 0;
		while (compindex < bin.size)
		{
			merged[newindex] = bin[compindex];
			++newindex;
			++compindex;
		}
		delete[] this->bits;
		this->bits = merged;
		this->size = this->size + bin.size;
	}
	else
	{
		*this = bin;
	}
}

void Binary::clear()
{
	if (this->size != 0)
	{
		this->size = 0;
		delete[] this->bits;
	}
}

void Binary::print()
{
	for (size_t i = 0; i < this->size; i++)
	{
		if (this->bits[i])
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}
}

bool* Binary::return_bits()
{
	if (this->size)
	{
		return this->bits;
	}
	else
	{
		return 0;
	}
}

size_t Binary::return_size()
{
	return this->size;
}

uint128 Binary::return_value()
{
	uint128 value = 0;
	for (size_t i = 0, val = 1, j = this->size - 1; i < this->size; ++i, val *= 2, --j)
	{
		if (this->bits[j])
		{
			value += val;
		}
	}
	return value;
}

void Binary::operator=(Binary& binary)
{
	this->clear();

	this->size = binary.size;
	this->bits = new bool[binary.size];

	for (size_t i = 0; i < binary.size; i++)
	{
		this->bits[i] = binary.bits[i];
	}
	
}

void Binary::operator=(std::vector<Binary>& b_list)
{
	this->clear();

	for (size_t i = 0; i < b_list.size(); i++)
	{
		this->size += b_list[i].return_size();
	}

	this->bits = new bool[this->size];

	for (size_t i = 0, j = 0; i < b_list.size(); i++)
	{
		for (size_t k = 0; k < b_list[i].return_size(); k++, j++)
		{
			this->bits[j] = b_list[i][k];
		}
	}
}

void Binary::operator=(unsigned char num)
{
	this->clear();
	this->bits = new bool[8];
	this->size = 8;
	std::bitset<8> bitset(num);

	for (size_t i = 0, j = this->size - 1; i < this->size; i++, j--)
	{
		this->bits[i] = bitset[j];
	}
}

void Binary::operator=(unsigned short num)
{
	this->clear();
	this->bits = new bool[16];
	this->size = 16;
	std::bitset<16> bitset(num);

	for (size_t i = 0, j = this->size - 1; i < this->size; i++, j--)
	{
		this->bits[i] = bitset[j];
	}
}

void Binary::operator=(unsigned int num)
{
	this->clear();
	this->bits = new bool[32];
	this->size = 32;
	std::bitset<32> bitset(num);

	for (size_t i = 0, j = this->size - 1; i < this->size; i++, j--)
	{
		this->bits[i] = bitset[j];
	}
}

void Binary::operator=(unsigned long long num)
{
	this->clear();
	this->bits = new bool[64];
	this->size = 64;
	std::bitset<64> bitset(num);

	for (size_t i = 0, j = this->size - 1; i < this->size; i++, j--)
	{
		this->bits[i] = bitset[j];
	}
}

void Binary::operator=(uint128& num)
{
	this->clear();
	this->bits = new bool[128];
	this->size = 128;

	uint128 MAX = MAX_8;
	++MAX;

	std::bitset<64> half((num / MAX).convert_to<size_t>());

	for (size_t i = 0, j = 63; i < 64; ++i, --j)
	{
		this->bits[i] = half[j];
	}
	half = (num % MAX).convert_to<size_t>();
	for (size_t i = 64, j = 63; i < 128; i++, --j)
	{
		this->bits[i] = half[j];
	}
}

bool Binary::operator[](size_t index)
{
	return this->bits[index];
}

Binary& Binary::operator++()
{
	for (size_t i = 0, j = this->size - 1; i < this->size; i++, j--)
	{
		if (this->bits[j])
		{
			this->bits[j] = false;
		}
		else
		{
			this->bits[j] = true;
			break;
		}
	}

	return *this;
}

Binary Binary::operator++(int)
{
	Binary temp(*this);
	++(*this);
	return temp;
}