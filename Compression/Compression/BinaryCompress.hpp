#pragma once

#include "BinaryConvert.hpp"

class BinaryCompress
{
private:
	size_t o_size;
	bool* o_bits;
	std::vector<int> compressed;
	signed char firstval;
	signed char size;

public:
	BinaryCompress();
	BinaryCompress(Binary& b_number);
	~BinaryCompress();

	void compress(bool* bits, size_t size);
	void doublecompress();
	void print();
	void printsize();
	size_t return_size();

	void operator=(Binary& b_num);
};

BinaryCompress::BinaryCompress()
{
	this->o_size = 0;
	this->o_bits = 0;
	this->firstval = 0;
	this->size = 0;
}

BinaryCompress::BinaryCompress(Binary& b_number)
{
	this->o_size = b_number.return_size();
	this->o_bits = b_number.return_bits();
	this->compress(this->o_bits, this->o_size);
}

BinaryCompress::~BinaryCompress()
{
	if (this->o_size != 0)
	{
		delete[] this->o_bits;
	}
}

void BinaryCompress::compress(bool* bits, size_t size)
{
	this->compressed.clear();

	bool state = bits[0];
	if (state == false)
	{
		this->compressed.push_back(1);
	}
	else
	{
		this->compressed.push_back(-1);
	}

	for (size_t i = 1; i < size; i++)
	{
		if (this->o_bits[i] == state)
		{
			if (state == false)
			{
				++this->compressed[this->compressed.size() - 1];
			}
			else
			{
				--this->compressed[this->compressed.size() - 1];
			}
		}
		else
		{
			state = !state;
			if (state == false)
			{
				this->compressed.push_back(1);
			}
			else
			{
				this->compressed.push_back(-1);
			}
		}
	}

	if (this->compressed[0] > 0)	//set size
	{
		this->size = this->compressed.size() - 1;
	}
	else
	{
		this->size = this->compressed.size() - 1;
		this->size = 0 - this->size;
	}
	this->firstval = this->compressed[0];

	this->doublecompress();
}

void BinaryCompress::doublecompress()
{
	std::vector<int> hold = this->compressed;
	this->compressed.clear();

	size_t start = 0;
	size_t end = 0;

	this->compressed.push_back(hold[1]);
	for (size_t i = 2, start = 1, end = 1, c_index = 0; i < hold.size() - 1; i++)
	{
		c_index = *this->compressed.end();
		if ((this->compressed[c_index] == -hold[i]) && ((hold[i] == 1) || hold[i] == -1))
		{
			if (this->compressed[c_index] > 0)
			{
				++this->compressed[c_index];
			}
			else
			{
				--this->compressed[c_index];
			}
		}
		else
		{
			this->compressed[c_index] -= 0;
			this->compressed.push_back(hold[i]);
		}
	}
}

void BinaryCompress::print()
{
	for (size_t i = 1; i < this->compressed.size() - 1; i++)
	{
		printf("%d ", this->compressed[i]);
	}
}

void BinaryCompress::printsize()
{
	printf("%i", this->compressed.size());
}

size_t BinaryCompress::return_size()
{
	return this->compressed.size();
}

void BinaryCompress::operator=(Binary& b_num)
{
	if (this->o_size != 0)
	{
		delete[] this->o_bits;
	}
	this->o_size = b_num.return_size();
	this->o_bits = b_num.return_bits();
	this->compress(this->o_bits, this->o_size);
}