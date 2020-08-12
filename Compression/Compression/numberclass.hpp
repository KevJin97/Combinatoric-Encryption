#pragma once

#include <iostream>

class Number
{
private:
	bool xtra;	//replace with unsigned char for more data. Refactor code for it later
	size_t size;
	size_t value;

public:
	Number();
	Number(size_t number);
	Number(size_t value, size_t size);
	Number(size_t value, size_t size, bool xtra);
	Number(const Number& number);
	~Number();

	void set(size_t value, size_t size);
	void set(size_t value, size_t size, bool xtra);

	void operator=(const Number& number);
	void operator+=(const Number& number);
	void operator-=(const Number& number);
	Number operator+(const Number& number);
	Number operator-(const Number& number);
	bool operator==(const Number& number);
	bool operator!=(const Number& number);
	bool operator>(const Number& number);
	bool operator<(const Number& number);
	bool operator>=(const Number& number);
	bool operator<=(const Number& number);
	Number& operator++();
	Number operator++(int);
	Number& operator--();
	Number operator--(int);

	void print()
	{
		std::cout << "( " << this->value << ", " << this->size << ", " << this->xtra << " )" << std::endl;
	}
};

Number::Number()
{
	this->xtra = false;
	this->size = 0;
	this->value = 0;
}

Number::Number(size_t number)
{
	this->xtra = false;
	this->size = 0;
	this->value = number;
}

Number::Number(const Number& number)
{
	this->xtra = number.xtra;
	this->size = number.size;
	this->value = number.value;
}

Number::Number(size_t value, size_t size)
{
	this->xtra = false;
	this->size = size;
	this->value = value;
}

Number::Number(size_t value, size_t size, bool xtra)
{
	this->xtra = xtra;
	this->size = size;
	this->value = value;
}

Number::~Number()
{

}

void Number::set(size_t value, size_t size)
{
	this->xtra = false;
	this->size = size;
	this->value = value;
}

void Number::set(size_t value, size_t size, bool xtra)
{
	this->xtra = xtra;
	this->size = size;
	this->value = value;
}

void Number::operator=(const Number& number)
{
	this->xtra = number.xtra;
	this->size = number.size;
	this->value = number.value;
}

void Number::operator+=(const Number& number)
{
	*this = *this + number;
}

void Number::operator-=(const Number& number)
{
	*this = *this - number;
}

Number Number::operator+(const Number& number)
{
	bool xtra = this->xtra || number.xtra;

	size_t power = this->size + number.size;
	if ((power < this->size) && (power < number.size))
	{
		xtra = !xtra;
	}

	size_t added = this->value + number.value;
	if ((added < this->value) && (added < number.value))
	{
		++power;
	}

	return Number(added, power, xtra);
}

Number Number::operator-(const Number& number)
{
	bool xtra = this->xtra || !number.xtra;

	size_t power = this->size - number.size;

	size_t added = this->value - number.value;
	if ((added > this->value) && (added > number.value))
	{
		--power;
		xtra = !xtra;
	}

	return Number(added, power, xtra);
}

bool Number::operator==(const Number& number)
{
	if ((this->value == number.value) && (this->size == number.size) && (this->xtra == number.xtra))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Number::operator!=(const Number& number)
{
	if ((this->value == number.value) && (this->size == number.size) && (this->xtra == number.xtra))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Number::operator>(const Number& number)
{
	if (this->xtra && !number.xtra)
	{
		return true;
	}
	else if (!this->xtra && number.xtra)
	{
		return false;
	}
	else
	{
		if (this->size > number.size)
		{
			return true;
		}
		else if (this->size < number.size)
		{
			return false;
		}
		else
		{
			if (this->value > number.value)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

bool Number::operator<(const Number& number)
{
	if (this->xtra && !number.xtra)
	{
		return false;
	}
	else if (!this->xtra && number.xtra)
	{
		return true;
	}
	else
	{
		if (this->size < number.size)
		{
			return true;
		}
		else if (this->size < number.size)
		{
			return false;
		}
		else
		{
			if (this->value < number.value)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

bool Number::operator>=(const Number& number)
{
	return !(*this < number);
}

bool Number::operator<=(const Number& number)
{
	return !(*this > number);
}

Number& Number::operator++()
{
	++this->value;
	if (this->value == 0)
	{
		++this->size;
		if (this->size == 0)
		{
			this->xtra = !this->xtra;
		}
	}

	return *this;
}

Number Number::operator++(int)
{
	Number temp(*this);
	++(*this);

	return temp;
}

Number& Number::operator--()
{
	if (this->value == 0)
	{
		if (this->size == 0)
		{
			this->xtra = !this->xtra;
		}
		--this->size;
	}
	--this->value;

	return *this;
}

Number Number::operator--(int)
{
	Number temp(*this);
	--(*this);

	return temp;
}