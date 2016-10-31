#include <bits/stdc++.h>
#include "my_vector.h"

my_vector::my_vector():
	_size(0)
{}

my_vector::short_number::short_number()
{
	for (size_t i = 0; i < amount_short; i++)
	{
		data[i] = 0;
	}
}

void my_vector::for_copy(size_t new_cap)
{
	lng.properties = static_cast <for_long_number*> (operator new (sizeof(for_long_number) + new_cap * sizeof(my_type)));
	lng.data = lng.properties->data;
	lng.properties->capacity = new_cap;
	lng.properties->ref_counter = 1;
}

void my_vector::copy_lng_srt(size_t new_cap)
{
	short_number tmp = srt;
	for_copy(new_cap);
	for (size_t i = 0; i < _size; i++)
		lng.data[i] = tmp.data[i];
}

void my_vector::copy_lng_lng(size_t new_cap)
{
	for_long_number* tmp = lng.properties;
	for_copy(new_cap);
	memcpy(lng.data, tmp->data, sizeof(my_type) * _size);
	--tmp->ref_counter;
	if (!tmp->ref_counter)
		operator delete (static_cast <void*> (tmp));
}

void my_vector::push_back(const my_type& src)
{
	if (_size < amount_short)
	{
		srt.data[_size++] = src;
	}
	else
	{
		if (_size == amount_short)
		{
			copy_lng_srt(2 * _size);
		}
		else
		{
			if (lng.properties->capacity == _size || lng.properties->ref_counter > 1)
			{
				size_t multiplier = (lng.properties->capacity == _size);
				copy_lng_lng(lng.properties->capacity << multiplier);
			}
		}
		lng.data[_size++] = src;
	}
}

void my_vector::pop_back()
{
	if (_size == amount_short + 1)
	{
		my_type tmp[amount_short];
		for (size_t i = 0; i < amount_short; i++)
			tmp[i] = lng.data[i];
		operator delete (static_cast <void*> (lng.properties));
		for (size_t i = 0; i < amount_short; i++)
			srt.data[i] = tmp[i];
	}
	else if (_size > amount_short)
	{
		if ((_size - 1) == lng.properties->capacity / 4 || lng.properties->ref_counter > 1)
		{
			size_t deleter = ((_size - 1) == lng.properties->capacity / 4);
			copy_lng_lng(lng.properties->capacity >> deleter);
		}	
	}
	--_size;
}

my_vector::my_type const& my_vector::operator[](size_t ind) const
{
	if (_size <= amount_short)
	{
		//std::cout << "Hey" << std::endl;
		return srt.data[ind];
	}
	else
	{
		//std::cout << "Huy" << std::endl;
		return lng.data[ind];
	}
}

my_vector::my_type& my_vector::operator[](size_t ind)
{
	if (_size > amount_short)
	{
		if (lng.properties->ref_counter > 1)
		{
			copy_lng_lng(lng.properties->capacity);
		}
		
		return lng.data[ind];
	}

	return srt.data[ind];
}



bool my_vector::operator==(const my_vector& src)
{
	if (src._size != _size)
		return false;
	if (_size <= amount_short)
	{
		for (size_t i = 0; i < _size; i++)
			if (srt.data[i] != src.srt.data[i])
				return false;
		return true;
	}
	else
	{
		for (size_t i = 0; i < _size; i++)
		{
			if (lng.data[i] != src.lng.data[i])
				return false;
		}
		return true;
	}
}

my_vector& my_vector::operator=(const my_vector& src)
{
	if (_size > amount_short)
	{
		--lng.properties->ref_counter;
		if (!lng.properties->ref_counter)
			operator delete(static_cast <void*> (lng.properties));
	}
	_size = src._size;
	for (size_t i = 0; i < amount_short; i++)
	{
		srt.data[i] = src.srt.data[i];
	}
	if (_size > amount_short)
	{
		++lng.properties->ref_counter;
	}
	return *this;
}

my_vector::my_vector(const my_vector& src):
	_size(src._size)
{
	for (size_t i = 0; i < amount_short; i++)
		srt.data[i] = src.srt.data[i];
	if (_size > amount_short)
	{
		++lng.properties->ref_counter;
	}
}

void my_vector::resize(size_t amount, my_type src)
{
	if (_size <= amount_short)
	{
		if (amount > amount_short)
			copy_lng_srt(amount);
	}
	else
	{
		if (amount <= amount_short)
		{
			--lng.properties->ref_counter;
			if (!lng.properties->ref_counter)
				operator delete(static_cast <void*> (lng.properties));
		}
		else
		{
			copy_lng_lng(amount);
		}
	}
	if (amount <= amount_short)
	{
		for (size_t i = _size; i < amount; i++)
		{
			srt.data[i] = src;
		}
	}
	else
	{
		//for (size_t i = _size; i < amount; i++)
		//{
			//lng.data[i] = src;
		//}
		if (amount > _size)
			memset(lng.data + _size, src, sizeof(my_type) * (amount - _size));
	}
	_size = amount;
}

void my_vector::clear()
{
	if (_size > amount_short)
	{
		--lng.properties->ref_counter;
		if (!lng.properties->ref_counter)
			operator delete(static_cast <void*> (lng.properties));
	}
	for (size_t i = 0; i < amount_short; i++)
	{
		srt.data[i] = 0;
	}
	_size = 0;
}


void my_vector::resize(size_t amount)
{
	resize(amount, 0);
}

my_vector::~my_vector()
{
	//std::cout << "Hay " << size << std::endl;
	if (_size > amount_short)
	{
		--lng.properties->ref_counter;
		//std::cout << "Here!" << std::endl;
		if (!lng.properties->ref_counter)
		{
			//std::cout << "Hello!" << std::endl;
			operator delete(static_cast <void*> (lng.properties));
		}
	}
}

my_vector::iterator my_vector::begin()
{
	return iterator(0, this);
}

my_vector::iterator my_vector::end()
{
	return iterator(_size, this);
}

my_vector::iterator::iterator(const iterator& it):
	ind(it.ind), par(it.par)
{}

my_vector::iterator& my_vector::iterator::operator++()
{
	//std::cout << ind << ' ' << par->_size << std::endl;
	assert(ind < par->_size);
	++ind;
	return *this;
}

my_vector::iterator my_vector::iterator::operator++(int)
{
	assert(ind < par->_size);
	iterator tmp = *this;
	++ind;
	return tmp;
}

my_vector::iterator& my_vector::iterator::operator--()
{
	assert(ind > 0);
	--ind;
	return *this;
}

my_vector::iterator my_vector::iterator::operator--(int)
{
	assert(ind > 0);
	iterator tmp = *this;
	--ind;
	return tmp;
}

my_vector::iterator& my_vector::iterator::operator+=(size_t rhs)
{
	assert(ind + rhs <= par->_size);
	ind += rhs;
	return *this;
}

my_vector::iterator& my_vector::iterator::operator-=(size_t rhs)
{
	assert((int)ind - (int)rhs >= 0);
	ind -= rhs;
	return *this;
}

my_vector::iterator::iterator(size_t _ind, my_vector* _another):
	ind(_ind), par(_another)
{}

my_vector::iterator my_vector::iterator::operator+(size_t rhs)
{
	assert(ind + rhs <= par->_size);
	return iterator(ind + rhs, par);
}

my_vector::iterator my_vector::iterator::operator-(size_t rhs)
{
	assert((int)ind - (int)rhs >= 0);
	return iterator(ind - rhs, par);
}

my_vector::my_type& my_vector::iterator::operator*() const
{
	return (*par)[ind];
}

bool operator==(my_vector::iterator const& a, my_vector::iterator const& b)
{
	assert(a.par == b.par);
	return (a.ind == b.ind);
}

bool operator!=(my_vector::iterator const& a, my_vector::iterator const& b)
{
	assert(a.par == b.par);
	return (a.ind != b.ind);
}

bool operator<(my_vector::iterator const& a, my_vector::iterator const& b)
{
	assert(a.par == b.par);
	return a.ind < b.ind;
}

bool operator<=(my_vector::iterator const& a, my_vector::iterator const& b)
{
	assert(a.par == b.par);
	return a.ind <= b.ind;
}

bool operator>(my_vector::iterator const& a, my_vector::iterator const& b)
{
	assert(a.par == b.par);
	return a.ind > b.ind;
}

bool operator>=(my_vector::iterator const& a, my_vector::iterator const& b)
{
	assert(a.par == b.par);
	return a.ind >= b.ind;
}

size_t operator-(my_vector::iterator const& a, my_vector::iterator const& b)
{
	assert(a.par == b.par);
	return a.ind - b.ind;
}

size_t my_vector::size() const
{
	return _size;
}


//using namespace std;

//void print(my_vector& tmp)
//{
	////cout << "Begin" << endl;
	//for (size_t i = 0; i < tmp.size(); i++)
	//{
		//cout << tmp[i] << ' ';
	//}
	//cout << endl;
	////cout << "End" << endl;
//}

//int main()
//{
	//my_vector vt;
	//for (size_t i = 0; i < 4; i++)
	//{
		//vt.push_back(i);
	//}
	//cout << "Start" << endl;
	//my_vector vt1(vt);
	//vt.resize(100);
	//print(vt);
	//print(vt1);
	//for (size_t i = 0; i < 4; i++)
	//{
		//vt1[i] = rand() % 100;
	//}
	////cout << "Hay " << (vt1.begin() + vt1.size() != vt1.end()) << endl;
	//sort(vt1.begin(), vt1.end());
	//cout << "Finish" << endl;
	//for (size_t i = 0; i < 4; i++)
	//{
		////print(vt);
		//vt.pop_back();
		////print(vt);
	//}
	//print(vt1);
	
	////cout << "Stop" << endl;
	//return 0;
//}
