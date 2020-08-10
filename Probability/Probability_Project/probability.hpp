#pragma once

#include <random>
#include <time.h>
#include <vector>
#include <algorithm>

class Probability
{
protected:
	size_t size;
	size_t* set_values;
	std::mt19937_64 random;

public:
	Probability();
	Probability(size_t* dataset);
	Probability(std::vector<size_t>& dataset);
	~Probability();

	static size_t factorial(size_t n);
	size_t random_gen(size_t range_start, size_t range_end);
	
	//Combination
	static size_t C(size_t n, size_t r);	//nCr
	static size_t C_r(size_t n, size_t r);	//with repetition
	static size_t* combine(size_t set_size, size_t size, size_t index);
	static size_t C_index(size_t set_size, size_t size, size_t* combinearray);

	//Permutation
	static size_t P(size_t n, size_t r);	//nPr
	static size_t P_r(size_t n, size_t r);	//with repeition allowed
	static size_t* permute(size_t size, size_t index);
	static size_t P_index(size_t* permutearray, size_t size);

	void operator=(size_t* dataset);
	void operator=(std::vector<size_t>& dataset);
};

Probability::Probability()
{
	this->size = 0;
	this->set_values = new size_t[1];
	this->random.seed(time(NULL));
}

Probability::Probability(size_t* dataset)
{
	this->size = sizeof(dataset) / sizeof(size_t);
	
	this->set_values = new size_t[size];
	for (size_t i = 0; i < this->size; i++)
	{
		this->set_values[i] = dataset[i];
	}
	this->random.seed(time(NULL));
}

Probability::Probability(std::vector<size_t>& dataset)
{
	this->size = dataset.size();

	this->set_values = new size_t[this->size];
	for (size_t i = 0; i < this->size; i++)
	{
		this->set_values[i] = dataset[i];
	}
	this->random.seed(time(NULL));
}

Probability::~Probability()
{
	delete[] this->set_values;
}

size_t Probability::factorial(size_t n)
{
	size_t fact = 1;
	for (size_t i = 2; i <= n; i++)
	{
		fact *= i;
	}
	return fact;
}

size_t Probability::random_gen(size_t range_start, size_t range_end)
{
	return (this->random() % range_end) + range_start;
}

size_t Probability::C(size_t n, size_t r)
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

size_t Probability::C_r(size_t n, size_t r)
{	
	if (r == 0)
	{
		return 0;
	}
	else
	{
		n += r - 1;

		return Probability::C(n, r);
	}
}

size_t* Probability::combine(size_t set_size, size_t size, size_t index)
{
	size_t** indices = new size_t*[size];
	size_t* value = new size_t[size];

	//set indices
	indices[size - 1] = new size_t[set_size];
	for (size_t i = 0; i < set_size; i++)
	{
		indices[size - 1][i] = 1;
	}
	for (size_t i = 1, j = size - 2; i < size; i++, j--)
	{
		indices[j] = new size_t[set_size];
		indices[j][set_size - 1] = 1;

		for (size_t k = 1, l = set_size - 1; k < set_size; k++, l--)
		{
			indices[j][l - 1] = indices[j + 1][l - 1] + indices[j][l];
		}
	}
	
	size_t indexstart = 0;
	size_t valuestart = 0;

	for (size_t loopindex = 0, loopcount = size; loopindex < loopcount; loopindex++)
	{
		for (size_t i = indexstart, j = indices[loopindex][i]; i < set_size; i++, j = indices[loopindex][i])
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

	for (size_t i = 0; i < size; i++)	//delete indices
	{
		delete[] indices[i];
	}
	delete[] indices;

	return value;
}

size_t Probability::C_index(size_t set_size, size_t size, size_t* combinearray)
{
	size_t index = 0;
	size_t** indices = new size_t * [size];

	//set indices
	indices[size - 1] = new size_t[set_size];
	for (size_t i = 0; i < set_size; i++)
	{
		indices[size - 1][i] = 1;
	}
	for (size_t i = 1, j = size - 2; i < size; i++, j--)
	{
		indices[j] = new size_t[set_size];
		indices[j][set_size - 1] = 1;

		for (size_t k = 1, l = set_size - 1; k < set_size; k++, l--)
		{
			indices[j][l - 1] = indices[j + 1][l - 1] + indices[j][l];
		}
	}

	size_t indexstart = 0;
	size_t valuestart = 0;

	for (size_t loopindex = 0, loopcount = size; loopindex < loopcount; loopindex++)
	{
		for (size_t i = indexstart, j = indices[loopindex][i]; i < set_size; i++, j = indices[loopindex][i])
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

	for (size_t i = 0; i < size; i++)	//delete indices
	{
		delete[] indices[i];
	}
	delete[] indices;

	return index;
}

size_t Probability::P(size_t n, size_t r)
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

size_t Probability::P_r(size_t n, size_t r)
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

size_t* Probability::permute(size_t size, size_t index)
{
	bool* p_used = new bool[size];
	for (size_t i = 0; i < size; i++)
	{
		p_used[i] = false;
	}

	size_t increment = Probability::factorial(size - 1);
	size_t* value = new size_t[size];

	for (size_t loopindex = 0, loopcount = size; loopindex < loopcount; loopindex++)
	{
		for (size_t j = 0, i = 0; i < size; i++, j++)
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

				if (size != loopindex + 1)
				{
					increment /= size - loopindex - 1;
				};

				break;
			}
		}
	}
	delete[] p_used;

	return value;
}

size_t Probability::P_index(size_t* permutearray, size_t size)
{
	bool* p_used = new bool[size];
	for (size_t i = 0; i < size; i++)
	{
		p_used[i] = false;
	}

	size_t fact = Probability::factorial(size);
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

void Probability::operator=(size_t* dataset)
{
	delete[] this->set_values;

	this->size = sizeof(dataset) / sizeof(size_t);

	this->set_values = new size_t[size];
	for (size_t i = 0; i < this->size; i++)
	{
		this->set_values[i] = dataset[i];
	}
}

void Probability::operator=(std::vector<size_t>& dataset)
{
	delete[] this->set_values;

	this->size = dataset.size();

	this->set_values = new size_t[this->size];
	for (size_t i = 0; i < this->size; i++)
	{
		this->set_values[i] = dataset[i];
	}
}