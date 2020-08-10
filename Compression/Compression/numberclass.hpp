#pragma once

class Number
{
private:
	unsigned short size;
	size_t value;

public:
	Number();
	Number(size_t number);
	Number(size_t value, unsigned short size);
	Number(const Number& number);
	~Number();

	void set(size_t value, unsigned short size);

	void operator=(const Number& number);
	void operator+=(const Number& number);
	void operator-=(const Number& number);
	void operator*=(const Number& number);
	void operator/=(const Number& number);
	void operator%=(const Number& number);
	Number operator+(const Number& number);
	Number operator-(const Number& number);
	Number operator*(const Number& number);
	Number operator/(const Number& number);
	Number operator%(const Number& number);
	bool operator==(const Number& number);
	bool operator!=(const Number& number);
	bool operator>(const Number& number);
	bool operator<(const Number& number);
	bool operator>=(const Number& number);
	bool operator<=(const Number& number);
	Number operator++();
	Number& operator++(int);
	Number operator--();
	Number& operator--(int);
};

Number::Number()
{
	this->size = 0;
	this->value = 0;
}

Number::Number(size_t number)
{
	this->size = 0;
	this->value = number;
}

Number::Number(const Number& number)
{
	this->size = number.size;
	this->value = number.size;
}

Number::Number(size_t value, unsigned short size)
{
	this->size = size;
	this->value = value;
}

Number::~Number()
{

}

void Number::set(size_t value, unsigned short size)
{
	this->size = size;
	this->value = value;
}

void Number::operator=(const Number& number)
{
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

void Number::operator*=(const Number& number)
{
	*this = *this * number;
}

void Number::operator/=(const Number& number)
{
	*this = *this / number;
}

void Number::operator%=(const Number& number)
{
	*this = *this % number;
}

Number Number::operator+(const Number& number)
{
	unsigned short power = this->size + number.size;

	size_t added = this->value + number.value;
	
	if ((added < this->value) && (added < number.value))
	{
		++power;
	}

	return Number(added, power);
}

Number Number::operator-(const Number& number)
{
	unsigned short power = this->size - number.size;

	size_t added = this->value - number.value;

	if (!((added < this->value) && (added < number.value)))
	{
		--power;
	}

	return Number(added, power);
}

Number Number::operator*(const Number& number)
{
	size_t value = this->value * number.value;

	return Number(0, 0);
}

Number Number::operator/(const Number& number)
{
	return Number(0, 0);
}

Number Number::operator%(const Number& number)
{
	return Number(0, 0);
}

bool Number::operator==(const Number& number)
{
	if ((this->value == number.value) && (this->size == number.size))
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
	if ((this->value == number.value) && (this->size == number.size))
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

bool Number::operator<(const Number& number)
{
	if (this->size > number.size)
	{
		return false;
	}
	else if (this->size < number.size)
	{
		return true;
	}
	else
	{
		if (this->value > number.value)
		{
			return false;
		}
		else
		{
			return true;
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

Number Number::operator++()
{
	++this->value;
	if (this->value == 0)
	{
		++this->size;
	}

	return *this;
}

Number& Number::operator++(int)
{
	Number temp(*this);
	++(*this);

	return temp;
}

Number Number::operator--()
{
	if (this->value == 0)
	{
		--this->size;
	}
	--this->value;

	return *this;
}

Number& Number::operator--(int)
{
	Number temp(*this);
	--(*this);

	return temp;
}