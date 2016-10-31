#include <bits/stdc++.h>
#include "big_integer.h"

enum {NEGATIVE, POSITIVE};

big_integer::big_integer(int a)
{
	sign = (a >= 0);
	long long now = a;
	if (now < 0) now = -now;
	number.push_back(now & max_value_good);
	now >>= shift;
	if (now > 0)
	{
		number.push_back(now);
	}
}

big_integer::big_integer()
{
	sign = POSITIVE;
	number.push_back(0);
}

big_integer::big_integer(big_integer const& tmp)
{
	sign = tmp.sign;
	number = tmp.number;
}

big_integer::~big_integer()
{
}

big_integer& big_integer::operator=(big_integer const& rhs)
{
	sign = rhs.sign;
	number = rhs.number;
	return *this;
}

int abs_compare(big_integer const& a, big_integer const& b)
{
	if (a.number.size() < b.number.size()) return -1;
	if (a.number.size() > b.number.size()) return 1;
	for (int i = (int)a.number.size() - 1; i >= 0; --i)
	{
		if (a.number[i] < b.number[i]) return -1;
		if (a.number[i] > b.number[i]) return 1;
	}
	return 0;
}

int compare(big_integer const& a, big_integer const& b)
{
	if (a.sign < b.sign) return -1;
	if (a.sign > b.sign) return 1;
	return (a.sign == POSITIVE) ? abs_compare(a, b) : -abs_compare(a, b);
}

bool operator >(big_integer const& a, big_integer const& b)
{
	return compare(a, b) > 0;
}

bool operator <(big_integer const& a, big_integer const& b)
{
	return compare(a, b) < 0;
}

bool operator !=(big_integer const& a, big_integer const& b)
{
	return compare(a, b) != 0;
}

bool operator ==(big_integer const& a, big_integer const& b)
{
	return compare(a, b) == 0;
}

bool operator <=(big_integer const& a, big_integer const& b)
{
	return compare(a, b) <= 0;
}

bool operator >=(big_integer const& a, big_integer const& b)
{
	return compare(a, b) >= 0;
}

big_integer& big_integer::operator +=(big_integer const& rhs)
{
	unsigned balance = 0;
	big_integer const* a = this;
	big_integer const* b = &rhs;
	if (b->sign > a->sign || b->sign < a->sign)
	{
		sign = (b->sign > a->sign) ? NEGATIVE : POSITIVE;
		int comp = abs_compare(*a, *b);
		if (comp < 0)
		{
			sign = (b->sign > a->sign) ? POSITIVE : NEGATIVE;
			std::swap(a, b);
		}
		if (comp == 0) sign = POSITIVE;
		for (size_t i = 0; i < a->number.size(); ++i)
		{
			int tmp = a->number[i] - balance - ((i < b->number.size()) ? b->number[i] : 0);
			if (i < number.size())
			{
				number[i] = (tmp >= 0) ? tmp : (tmp + max_value);
			}
			else
			{
				number.push_back((tmp >= 0) ? tmp : (tmp + max_value));
			}
			balance = (tmp < 0);
		}
		for (int i = (int)number.size() - 1; number[i] == 0 && i > 0; --i)
		{
			number.pop_back();
		}
		return *this;
	}
	if (a->number.size() < b->number.size())
	{
		std::swap(a, b);
	}
	for (size_t j = 0; j < a->number.size(); j++)
	{
		int tmp = a->number[j] + balance + ((j < b->number.size()) ? b->number[j] : 0);
		if (j < number.size())
		{
			number[j] = tmp & max_value_good;
		}
		else
		{
			number.push_back(tmp & (int)max_value_good);
		}
		balance = (tmp > (int)max_value_good);
	}
	if (balance)
	{
		number.push_back(balance);
	}
	return *this;
}

big_integer& big_integer::operator -=(big_integer const& rhs)
{
	sign = (1 - sign);
	*this += rhs;
	sign = (1 - sign);
	if (number.size() == 1 && number[0] == 0) sign = POSITIVE;
	return *this;
}

big_integer& big_integer::operator *=(int rhs)
{
	if (number.size() == 1 && number[0] == 0)
	{
		return *this;
	}
	if (rhs == 0)
	{
		sign = POSITIVE;
		number.clear();
		number.push_back(0);
		return *this;
	}
	if (rhs == 1)
	{
		return *this;
	}
	if (rhs == -1)
	{
		sign = 1 - sign;
		return *this;
	}
	unsigned long long balance = 0;
	if (sign == (rhs > 0)) sign = POSITIVE;
	else sign = NEGATIVE;
	long long rhs1 = rhs;
	rhs1 = std::abs(rhs1);
	for (size_t i = 0; i < number.size(); ++i)
	{
		unsigned long long tmp = (unsigned long long)number[i] * rhs1 + balance;
		number[i] = tmp & max_value_good;
		balance = tmp >> shift;
	}
	if (balance)
	{
		number.push_back(balance);
	}
	return *this;
}

int big_integer::operator /=(int rhs)
{
	if (sign == (rhs > 0)) sign = POSITIVE;
	else sign = NEGATIVE;
	long long tmp1 = rhs;
	unsigned long long rhs1 = std::abs(tmp1);
	unsigned long long balance = 0;
	for (int i = (int)number.size() - 1; i >= 0; --i)
	{
		unsigned long long tmp = ((balance << shift) + number[i]) / rhs1;
		balance = ((balance << shift) + number[i]) - tmp * rhs1;
		number[i] = tmp;
	}
	for (int i = (int)number.size() - 1; number[i] == 0 && i > 0; --i)
	{
		number.pop_back();
	}
	return balance;
}

big_integer& big_integer::operator +=(unsigned a)
{
	unsigned balance = a;
	for (size_t i = 0; balance != 0 && i < number.size(); ++i)
	{
		unsigned tmp = number[i] + balance;
		number[i] = tmp & max_value_good;
		balance = tmp >> shift;
	}
	if (balance)
	{
		number.push_back(balance);
	}
	return *this;
}

big_integer::big_integer(std::string const& str)
{
	sign = POSITIVE;
	big_integer tmp;
	size_t i = 0;
	if (str[0] == '-')
	{
		sign = NEGATIVE;
		++i;
	}
	for (; i < str.size(); ++i)
	{
		tmp *= 10;
		tmp += (str[i] - '0');
	}
	this->number = tmp.number;
}

std::string to_string(big_integer const& a)
{
	std::string ans = "";
	if (a.number.size() == 1 && a.number[0] == 0) {
		ans += '0';
		return ans;
	}
	big_integer tmp = a;
	while (tmp.number.size() != 1 || tmp.number[0] != 0)
	{
		ans += (tmp /= 10) + '0';
	}
	if (a.sign == NEGATIVE)
	{
		ans += '-';
	}
	reverse(ans.begin(), ans.end());
	return ans;
}

std::ostream& operator <<(std::ostream& s, big_integer const& a)
{
	return (s << to_string(a));
}

void big_integer::reverse_bits()
{
	for (size_t i = 0; i < number.size(); ++i)
	{
		number[i] = ((~number[i]) & max_value_good);
	}
}

void make_binary_convenient2(big_integer& a, big_integer& b)
{
	big_integer* c = &a;
	big_integer* d = &b;
	if (c->number.size() < d->number.size())
	{
		std::swap(c, d);
	}
	c->number.push_back(0);
	while (c->number.size() != d->number.size())
	{
		d->number.push_back(0);
	}
	if (c->sign == NEGATIVE)
	{
		c->reverse_bits();
		++(*c);
	}
	if (d->sign == NEGATIVE)
	{
		d->reverse_bits();
		++(*d);
	}
}

void big_integer::make_binary_convenient()
{
	number.push_back(0);
	if (sign == NEGATIVE)
	{
		reverse_bits();
		++(*this);
	}
}

void big_integer::return_good_form()
{
	if (number[(int)number.size() - 1] & (1 << (shift - 1)))
	{
		sign = NEGATIVE;
		this->reverse_bits();
		while (!number[(int)number.size() - 1] && number.size() > 1)
		{
			number.pop_back();
		}
		++(*this);
	}
	else
	{
		sign = POSITIVE;
	}
	while (!number[(int)number.size() - 1] && number.size() > 1)
	{
		number.pop_back();
	}
}

big_integer& big_integer::operator >>=(int rhs)
{
	if (sign == NEGATIVE)
	{
		make_binary_convenient();
	}
	int last = (int)number.size() - 1;
	int bit_size = (int)number.size() * shift;
	for (int i = shift - 1; !(number[last] & (1 << i)) && i >= 0; i--)
	{
		--bit_size;
	}
	if (rhs >= bit_size)
	{
		number.clear();
		number.push_back(0);
	}
	else if (rhs % shift == 0)
	{
		int t = rhs / shift;
		for (int i = t; i < (int)number.size(); ++i)
		{
			number[i - t] = number[i];
		}
		number.pop_back();
	}
	else
	{
		int sh = rhs % shift;
		unsigned balance = (number[(int)number.size() - 1] >> sh);
		if (sign == NEGATIVE)
		{
			balance = (number[(int)number.size() - 1] >> sh) | ((~((1 << (shift - sh)) - 1)) & max_value_good);
		}
		int t = rhs / shift;
		if (sign == POSITIVE) {
			for (int i = t; i < (int)number.size() - 1; ++i)
			{
				number[i - t] = ((number[i] >> sh) | (((number[i + 1] << (shift - sh)) & max_value_good)));
			}
			number.resize((int)number.size() - 1 - t + (balance > 0));
			if (balance > 0)
			{
				number[(int)number.size() - 1] = balance;
			}
		}
		else
		{
			for (int i = t; i < (int)number.size() - 1; ++i)
			{
				unsigned buf = ((number[i] >> sh) | (((number[i + 1] << (shift - sh)) & max_value_good)));
				number[i] = max_value_good;
				number[i - t] = buf;
			}
			if (balance > 0)
			{
				number[(int)number.size() - 1 - t] = balance;
			}
		}
	}
	if (sign == NEGATIVE)
	{
		return_good_form();
	}
	return *this;
}

big_integer& big_integer::operator <<=(int rhs)
{
	if (rhs % shift == 0)
	{
		std::reverse(number.begin(), number.end());
		number.resize((int)number.size() + rhs / shift, 0);
		std::reverse(number.begin(), number.end());
	}
	else
	{
		int sz = rhs / shift;
		int balance = rhs % shift;
		int last = (int)number.size() - 1;
		int bit_size = (int)number.size() * shift;
		for (int i = shift - 1; !(number[last] & (1 << i)) && i >= 0; i--)
		{
			--bit_size;
		}
		int now_sz = (bit_size + balance) / shift + (((bit_size + balance) % shift) > 0);
		if (now_sz > (int)number.size())
		{
			number.push_back(number[(int)number.size() - 1]);
			for (int i = (int)number.size() - 2; i >= 0; --i)
			{
				number[i + 1] = number[i];
			}
			number[0] = 0;
			unsigned bal = 0;
			for (int i = 0; i < (int)number.size() - 1; ++i)
			{
				number[i] = ((number[i + 1] << balance) & max_value_good) | bal;
				bal = (number[i + 1] >> (shift - balance));
			}
			number[(int)number.size() - 1] = bal;
		}
		else
		{
			unsigned bal = 0;
			for (int i = 0; i < now_sz; ++i)
			{
				unsigned bal1 = bal;
				bal = number[i] >> (shift - balance);
				number[i] = ((number[i] << balance) & max_value_good) | bal1;
			}
		}
		std::reverse(number.begin(), number.end());
		number.resize(number.size() + sz, 0);
		std::reverse(number.begin(), number.end());
	}
	return *this;
}

big_integer& big_integer::operator *=(big_integer const& rhs)
{
	big_integer tmp(0);
	tmp.number.resize(number.size() + rhs.number.size());
	if (sign == (rhs.sign == POSITIVE)) sign = POSITIVE;
	else sign = NEGATIVE;
	for (size_t i = 0; i < rhs.number.size(); ++i)
	{
		unsigned long long balance = 0;
		long long rhs1 = rhs.number[i];
		rhs1 = std::abs(rhs1);
		for (size_t j = 0; j < number.size(); ++j)
		{
			unsigned long long tmp1 = (unsigned long long)number[j] * rhs1 + balance + tmp.number[j + i];
			tmp.number[j + i] = tmp1 & max_value_good;
			balance = tmp1 >> shift;
		}
		if (balance)
		{
			tmp.number[i + number.size()] = balance;
		}
	}
	while (tmp.number[(int)tmp.number.size() - 1] == 0)
	{
		tmp.number.pop_back();
	}
	number = tmp.number;
	return *this;
}

big_integer big_integer::operator +() const
{
	return *this;
}

big_integer big_integer::operator -() const
{
	big_integer tmp (*this);
	tmp.sign = 1 - (tmp.sign);
	if (tmp.number.size() == 1 && tmp.number[0] == 0) tmp.sign = 1;
	return tmp;
}

big_integer& big_integer::operator ++()
{
	*this += 1;
	return *this;
}

big_integer big_integer::operator ++(int)
{
	*this += 1;
	return *this;
}

big_integer& big_integer::operator --()
{
	*this -= 1;
	return *this;
}

big_integer big_integer::operator --(int)
{
	*this -= 1;
	return *this;
}

big_integer operator +(big_integer a, big_integer const& b)
{
	return (a += b);
}

big_integer operator -(big_integer a, big_integer const& b)
{
	return (a -= b);
}

big_integer operator *(big_integer a, big_integer const& b)
{
	return (a *= b);
}

void bin(int a)
{
	bin(a / 2);
	std::cout << a % 2 << std::endl;
}

big_integer& big_integer::operator /=(big_integer const& rhs)
{
	int res = abs_compare(rhs, *this);
	if (res == 1)
	{
		number.clear();
		sign = POSITIVE;
		number.push_back(0);
		return *this;
	}
	else if (res == 0) {
		number.clear();
		if (rhs.sign == NEGATIVE) sign = (1 - sign);
		number.push_back(1);
		return *this;
	}
	if (rhs.number.size() == 1)
	{
		*this /= rhs.number[0];
		if (rhs.sign == NEGATIVE)
		{
			sign = 1 - sign;
		}
		return *this;
	}
	bool last_sign = sign;
	sign = POSITIVE;
	big_integer tmp = rhs;
	tmp.sign = POSITIVE;
	big_integer tmp2;
	tmp <<= ((int)number.size() - (int)rhs.number.size()) * shift;
	if (tmp > *this)
	{
		tmp >>= shift;
		tmp2.number.resize((int)number.size() - (int)rhs.number.size());
	}
	else
	{
		tmp2.number.resize((int)number.size() - (int)rhs.number.size() + 1);
	}
	for (int i = (int)tmp2.number.size() - 1; i >= 0; i--)
	{
		unsigned l = 0, r = max_value;
		while (l + 1 != r)
		{
			unsigned m = (l + r) / 2;
			tmp *= m;
			if (tmp <= *this)
			{
				l = m;
			}
			else
			{
				r = m;
			}
			tmp /= m;
		}
		tmp2.number[i] = l;
		if (l != 0)
		{
			tmp *= l;
			*this -= tmp;
			tmp /= l;
		}
		tmp >>= shift;
	}
	if (rhs.sign == NEGATIVE)
	{
		sign = 1 - last_sign;
	}
	else
	{
		sign = last_sign;
	}
	number = tmp2.number;
	return *this;
}

big_integer big_integer::operator ~() const
{
	big_integer tmp = *this;
	tmp.sign = 1 - tmp.sign;
	if (tmp.number.size() == 1 && tmp.number[0] == 0) tmp.sign = 1;
	--tmp;
	return tmp;
}

big_integer operator /(big_integer a, big_integer const& b)
{
	return (a /= b);
}

big_integer operator >>(big_integer a, int b)
{
	return (a >>= b);
}

big_integer operator <<(big_integer a, int b)
{
	return (a <<= b);
}

big_integer& big_integer::operator %=(big_integer const& rhs)
{
	big_integer tmp = *this / rhs;
	*this -= tmp * rhs;
	return *this;
}

big_integer operator %(big_integer a, big_integer const& b)
{
	return (a %= b);
}

big_integer& big_integer::operator &=(big_integer const& rhs)
{
	big_integer tmp = rhs;
	make_binary_convenient2(*this, tmp);
	for (size_t i = 0; i < number.size(); ++i)
	{
		number[i] &= tmp.number[i];
	}
	this->return_good_form();
	return *this;
}

big_integer& big_integer::operator |=(big_integer const& rhs)
{
	big_integer tmp = rhs;
	make_binary_convenient2(*this, tmp);
	for (size_t i = 0; i < number.size(); ++i)
	{
		number[i] |= tmp.number[i];
	}
	this->return_good_form();
	return *this;
}

big_integer& big_integer::operator ^=(big_integer const& rhs)
{
	big_integer tmp = rhs;
	make_binary_convenient2(*this, tmp);
	for (size_t i = 0; i < number.size(); ++i)
	{
		number[i] ^= tmp.number[i];
	}
	this->return_good_form();
	return *this;
}

big_integer operator &(big_integer a, big_integer const& b)
{
	return (a &= b);
}

big_integer operator |(big_integer a, big_integer const& b)
{
	return (a |= b);
}

big_integer operator ^(big_integer a, big_integer const& b)
{
	return (a ^= b);
}

big_integer abs(big_integer a)
{
	a.sign = POSITIVE;
	return a;
}

//int main()
//{
	//big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	//big_integer b(                                                     "100000000000000000000000000000000000000");
	//big_integer c("10000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000");
	//std::cout << a << ' ' << b << std::endl;
	
	//std::cout << a + b << std::endl;
//}
