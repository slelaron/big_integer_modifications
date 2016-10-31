#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <bits/stdc++.h>


struct my_vector
{
	typedef unsigned my_type;
	static const size_t amount_short = 4;
	
	my_type& operator[](size_t);
	my_type const& operator[](size_t) const;
	void push_back(const my_type& src);
	void pop_back();
	my_vector& operator=(const my_vector& src);
	bool operator==(const my_vector& src);
	my_vector(const my_vector& src);
	my_vector();
	~my_vector();
	void resize(size_t);
	void resize(size_t, my_type src);
	void clear();

	size_t size() const;

	struct iterator: public std::iterator<std::random_access_iterator_tag, my_type>
	{	
		iterator(const iterator&);
		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		iterator& operator-=(size_t);
		iterator& operator+=(size_t);
		iterator operator+(size_t);
		iterator operator-(size_t);
		my_type& operator*() const;

		friend my_vector;

		friend bool operator==(my_vector::iterator const&, my_vector::iterator const&);
		friend bool operator!=(my_vector::iterator const&, my_vector::iterator const&);
		friend bool operator<=(my_vector::iterator const&, my_vector::iterator const&);
		friend bool operator<(my_vector::iterator const&, my_vector::iterator const&);
		friend bool operator>=(my_vector::iterator const&, my_vector::iterator const&);
		friend bool operator>(my_vector::iterator const&, my_vector::iterator const&);
		friend size_t operator -(my_vector::iterator const&, my_vector::iterator const&);
		
		private:
		size_t ind;
		my_vector* par;
		iterator(size_t, my_vector*);
	};

	iterator begin();
	iterator end();
	
	private:

	size_t _size;

	inline void for_copy(size_t new_cap);
	inline void copy_lng_lng(size_t new_cap);
	inline void copy_lng_srt(size_t new_cap);
	
	struct for_long_number
	{
		size_t ref_counter;
		size_t capacity;
		my_type data[];
	};
	struct long_number
	{
		for_long_number* properties;
		my_type* data;
	};

	struct short_number
	{
		my_type data[amount_short];
		short_number();
	};
	
	union
	{
		short_number srt;
		long_number lng;
	};
};

#endif
