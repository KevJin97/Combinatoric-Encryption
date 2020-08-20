#pragma once

#include <vector>
#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include "BinaryConvert.hpp"

template<typename T>
class BitPack
{
private:
	std::vector<Binary> binary_list;
	std::vector<uint128> values;
	unsigned short mode;
	Binary b_mode;
	Binary original;
	size_t split;

	size_t countzeros(Binary& binary);
	void pack();

public:
	BitPack();
	BitPack(unsigned char num);
	BitPack(unsigned short num);
	BitPack(unsigned int num);
	BitPack(unsigned long long num);
	BitPack(uint128& num);
	~BitPack();

	uint128 unpack(std::vector<T> values, unsigned char mode, size_t size);
	unsigned char return_mode();
	std::vector<uint128> return_values();
	void print();
	void clear();

	void operator=(Binary& binary);
	void operator=(unsigned char num);		//8 bit
	void operator=(unsigned short num);		//16 bit
	void operator=(unsigned int num);		//32 bit
	void operator=(unsigned long long num);	//64 bit
	void operator=(uint128& num);			//128 bit
};

template<typename T>
BitPack<T>::BitPack()
{
	this->mode = 0;
	this->split = 0;
}

template<typename T>
BitPack<T>::BitPack(unsigned char num)
{
	this->split = 0;
	*this = num;
}

template<typename T>
BitPack<T>::BitPack(unsigned short num)
{
	this->split = 0;
	*this = num;
}

template<typename T>
BitPack<T>::BitPack(unsigned int num)
{
	this->split = 0;
	*this = num;
}

template<typename T>
BitPack<T>::BitPack(unsigned long long num)
{
	this->split = 0;
	*this = num;
}

template<typename T>
BitPack<T>::BitPack(uint128& num)
{
	this->split = 0;
	*this = num;
}

template<typename T>
BitPack<T>::~BitPack()
{
	this->clear();
}

template<typename T>
size_t BitPack<T>::countzeros(Binary& binary)
{
	size_t num = 0;
	
	while (!binary[num] && num < binary.return_size())
	{
		++num;
	}

	return num;
}

template<typename T>
void BitPack<T>::pack()
{
	this->binary_list = this->original.split(this->split);
	size_t bitsize = binary_list[0].return_size();

	//bool* b_array = new bool[this->binary_list.size()];
	bool* b_array = new bool[16];	//hard-coded. Generalized above

	for (size_t i = 0; i < 16; ++i)	//hard-coding 16-bits for unsigned char. May be modified later
	{
		if (this->countzeros(this->binary_list[i]) == bitsize)
		{
			b_array[i] = false;
		}
		else
		{
			b_array[i] = true;
		}
	}

	this->b_mode.set(b_array, this->binary_list.size());
	this->mode = 0;
	size_t valuesize = 0;

	for (size_t i = 0, val = 1, j = this->binary_list.size() - 1; i < this->binary_list.size(); ++i, val *= 2, --j)
	{
		if (b_array[j])
		{
			this->mode += val;
			++valuesize;
		}
	}

	this->values.resize(valuesize);
	for (size_t i = 0, j = 0; i < this->binary_list.size(); ++i)
	{
		if (b_array[i])
		{
			this->values[j] = this->binary_list[i].return_value();
			++j;
		}
	}

	delete[] b_array;
}

template<typename T>
uint128 BitPack<T>::unpack(std::vector<T> values, unsigned char mode, size_t size)
{
	size *= 8;
	Binary b_mode = mode;

	Binary value;

	for (size_t i = 0, j = 0; i < b_mode.return_size(); ++i)
	{
		if (b_mode[i])
		{
			Binary bin(values[j], size);
			value.merge(bin);
			++j;
		}
		else
		{
			Binary bin(0, size);
			value.merge(bin);
		}
	}
	return value.return_value();
}

template<typename T>
unsigned char BitPack<T>::return_mode()
{
	return this->mode;
}

template<typename T>
std::vector<uint128> BitPack<T>::return_values()
{
	return this->values;
}

template<typename T>
void BitPack<T>::print()
{
	std::cout << "Mode: " << (int)this->mode << std::endl;
	for (size_t i = 0; i < this->binary_list.size(); i++)
	{
		this->binary_list[i].print();
		std::cout << std::endl;
	} std::cout << std::endl;
}

template<typename T>
void BitPack<T>::clear()
{
	this->values.clear();
	this->binary_list.clear();
	this->mode = 0;
	this->original.clear();
}

template<typename T>
void BitPack<T>::operator=(Binary& binary)
{
	switch (binary.return_size())
	{
	case 8:
		this->split = 1;
		break;

	case 16:
		this->split = 2;
		break;

	case 32:
		this->split = 4;
		break;

	default:
		this->split = 8;
		break;
	}
	this->clear();
	this->original = binary;
	this->pack();
}

template<typename T>
void BitPack<T>::operator=(unsigned char num)
{
	this->split = 1;
	this->clear();
	this->original = num;
	this->pack();
}

template<typename T>
void BitPack<T>::operator=(unsigned short num)
{
	this->split = 2;
	this->clear();
	this->original = num;
	this->pack();
}

template<typename T>
void BitPack<T>::operator=(unsigned int num)
{
	this->split = 4;
	this->clear();
	this->original = num;
	this->pack();
}

template<typename T>
void BitPack<T>::operator=(unsigned long long num)
{
	this->split = 8;
	this->clear();
	this->original = num;
	this->pack();
}

template<typename T>
void BitPack<T>::operator=(uint128& num)
{
	this->split = 16;
	this->clear();
	this->original = num;
	this->pack();
}