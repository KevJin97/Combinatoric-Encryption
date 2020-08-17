#pragma once

#include <vector>
#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include "BinaryConvert.hpp"

struct Base4
{
	std::vector<unsigned char> values;
};

Base4 base10tobase4(unsigned char number)
{
	Base4 base4;

	while (number != 0)
	{
		base4.values.push_back(number % 4);
		number /= 4;
	}
	std::reverse(base4.values.begin(), base4.values.end());

	return base4;
}

unsigned char base4tobase10(Base4& number)
{
	unsigned char result, value = 0;

	for (size_t i = 0, j = number.values.size() - 1, k = 0; i < number.values.size(); i++, j--, k = 0)
	{
		result = 1;
		do
		{
			result *= 4;
			++k;
		} while (k < j);

		value += number.values[i] * result;
	}

	return value;
}


class BitPack
{
private:
	std::vector<Binary> binary_list;
	unsigned char mode;
	Binary original;

	size_t countzeros(Binary& binary);
	void pack();	//specially written to deal with 128 bit numbers. rewrite for gen-purp later
	unsigned char getmode(Binary& binary);

public:
	BitPack();
	BitPack(unsigned char num);
	BitPack(unsigned short num);
	BitPack(unsigned int num);
	BitPack(unsigned long long num);
	BitPack(uint128 num);
	~BitPack();

	void print();
	void clear();

	void operator=(Binary& binary);
	void operator=(unsigned char num);		//8 bit
	void operator=(unsigned short num);		//16 bit
	void operator=(unsigned int num);		//32 bit
	void operator=(unsigned long long num);	//64 bit
	void operator=(uint128 num);			//128 bit
};

BitPack::BitPack()
{
	this->mode = 0;
}

BitPack::BitPack(unsigned char num)
{
	*this = num;
}

BitPack::BitPack(unsigned short num)
{
	*this = num;
}

BitPack::BitPack(unsigned int num)
{
	*this = num;
}

BitPack::BitPack(unsigned long long num)
{
	*this = num;
}

BitPack::BitPack(uint128 num)
{
	*this = num;
}

BitPack::~BitPack()
{
	this->clear();
}

size_t BitPack::countzeros(Binary& binary)
{
	size_t num = 0;
	
	while (!binary[num] && num < binary.return_size())
	{
		++num;
	}

	return num;
}

void BitPack::pack()
{
	//TODO: make sure to find a way to remove excess redundant zeros
	std::vector<Binary> firstsplit = this->original.split();
	std::vector<Binary> secondsplit = firstsplit[0].split();
	secondsplit.resize(4);
	firstsplit = firstsplit[1].split();
	secondsplit[2] = firstsplit[0];
	secondsplit[3] = firstsplit[1];

	Base4 index;
	index.values.resize(4);

	for (size_t i = 0; i < 4; i++)
	{
		index.values[i] = this->getmode(secondsplit[i]);
		
		switch (index.values[i])
		{
		case 0:
			secondsplit[i].resize(8);
			break;

		case 1:
			secondsplit[i].resize(16);
			break;

		case 2:
			secondsplit[i].resize(32);
			break;

		case 3:
			secondsplit[i].resize(64);
			break;
		}
	}

	this->mode = base4tobase10(index);
	this->binary_list = secondsplit;
}

unsigned char BitPack::getmode(Binary& binary)
{
	unsigned char bitsize = this->countzeros(binary);
	bitsize = binary.return_size() - bitsize;	//count non-redundant bits
	
	//fix later
	/*
	if (bitsize == 0)
	{
		return 0;
	}
	else*/ if (bitsize <= 8)		//8 bits (char)
	{
		//return 1;
		return 0;
	}
	else if (bitsize <= 16)	//16 bits (short)
	{
		//return 2;
		return 1;
	}
	else if (bitsize <= 32)	//32 bits (int)
	{
		//return 3;
		return 2;
	}
	else if (bitsize <= 64)	//64 bits (long)
	{
		//return 4;
		return 3;
	}
	else					//128 bits (uint128)
	{
		//return 5;
		return 3;
	}
}

void BitPack::print()
{
	std::cout << "Mode: " << (int)this->mode << std::endl;
	for (size_t i = 0; i < this->binary_list.size(); i++)
	{
		this->binary_list[i].print();
		std::cout << std::endl;
	} std::cout << std::endl;
}

void BitPack::clear()
{
	this->binary_list.clear();
	this->mode = 0;
	this->original.clear();
}

void BitPack::operator=(Binary& binary)
{
	this->clear();
	this->original = binary;
	this->pack();
}

void BitPack::operator=(unsigned char num)
{
	this->clear();
	this->original = num;
	this->pack();
}

void BitPack::operator=(unsigned short num)
{
	this->clear();
	this->original = num;
	this->pack();
}

void BitPack::operator=(unsigned int num)
{
	this->clear();
	this->original = num;
	this->pack();
}

void BitPack::operator=(unsigned long long num)
{
	this->clear();
	this->original = num;
	this->pack();
}

void BitPack::operator=(uint128 num)
{
	this->clear();
	this->original = num;
	this->pack();
}