#pragma once

#include "numberclass.hpp"

struct Index
{
	unsigned int start;
	Number value;
};

class Compression
{
private:
	size_t** indices;
	size_t set_size;
	size_t size;

public:
	Compression();
	Compression(size_t set_size, size_t size);
	~Compression();

	size_t factorial(size_t n);
	size_t C(size_t n, size_t r);
	size_t C_r(size_t n, size_t r);
	size_t P(size_t n, size_t r);
	size_t P_r(size_t n, size_t r);
	void set_indices(size_t set_size, size_t size);
	void delete_indices();
	size_t* combine(size_t index);
	size_t* permute(size_t index);
	size_t C_index(size_t* combinearray);
	size_t P_index(size_t* permutearray);
	void set_value(size_t set_size, size_t size);
	//compress()
	//decompress()
};

Compression::Compression()
{
	this->indices = NULL;
	this->set_size = 0;
	this->size = 0;
}

Compression::Compression(size_t set_size, size_t size)
{
	this->set_size = set_size;
	this->size = size;
	this->set_indices(set_size, size);
}

Compression::~Compression()
{
	this->delete_indices();
}

size_t Compression::factorial(size_t n)
{
	size_t fact = 1;
	for (size_t i = 2; i <= n; i++)
	{
		fact *= i;
	}
	return fact;
}

size_t Compression::C(size_t n, size_t r)
{
	size_t c = 0;

	if (n >= r)
	{
		size_t* prev;
		size_t* p_pascal = new size_t[2];
		p_pascal[0] = 1;
		p_pascal[1] = 1;

		for (size_t i = 2; i <= n; i++)
		{
			prev = p_pascal;

			p_pascal = new size_t[i + 1];
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

size_t Compression::C_r(size_t n, size_t r)
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

size_t Compression::P(size_t n, size_t r)
{
	size_t p = 1;

	if (n >= r)
	{
		for (size_t m = n - r; n > m; n--)
		{
			p *= n;
		}
	}

	return p;
}

size_t Compression::P_r(size_t n, size_t r)
{
	size_t pr = 1;
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

	this->indices = new size_t*[size];

	this->indices[size - 1] = new size_t[set_size];
	for (size_t i = 0; i < set_size; i++)
	{
		this->indices[size - 1][i] = 1;
	}
	for (size_t i = 1, j = size - 2; i < size; i++, j--)
	{
		this->indices[j] = new size_t[set_size];
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

size_t* Compression::combine(size_t index)
{
	size_t* value = new size_t[this->size];

	size_t indexstart = 0;
	size_t valuestart = 0;

	for (size_t loopindex = 0, loopcount = this->size; loopindex < loopcount; loopindex++)
	{
		for (size_t i = indexstart, j = this->indices[loopindex][i]; i < this->set_size; i++, j = this->indices[loopindex][i])
		{
			if (index < valuestart + j)
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

size_t* Compression::permute(size_t index)
{
	bool* p_used = new bool[this->size];
	for (size_t i = 0; i < this->size; i++)
	{
		p_used[i] = false;
	}

	size_t increment = this->factorial(this->size - 1);
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

size_t Compression::C_index(size_t* combinearray)
{
	size_t index = 0;

	size_t indexstart = 0;
	size_t valuestart = 0;

	for (size_t loopindex = 0, loopcount = this->size; loopindex < loopcount; loopindex++)
	{
		for (size_t i = indexstart, j = this->indices[loopindex][i]; i < this->set_size; i++, j = this->indices[loopindex][i])
		{
			if (combinearray[loopindex] == i)
			{
				index = valuestart;
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

size_t Compression::P_index(size_t* permutearray)
{
	size_t size = this->size;

	bool* p_used = new bool[this->size];
	for (size_t i = 0; i < this->size; i++)
	{
		p_used[i] = false;
	}

	size_t fact = this->factorial(size);
	size_t increment = fact / size;
	size_t index = 0;

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