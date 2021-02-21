#pragma once;

#include <memory.h>
#include <initializer_list>

#include "../BasicIterator/BasicIterator.h"

namespace UltimaAPI
{
	template <typename type>
	class BasicString;

	using String = BasicString<char>;
	using WString = BasicString<wchar_t>;
};

// fix length (it's used + 1)
template <typename type>
class  UltimaAPI::BasicString
{
	double mul_alloc = 1.6487; // sqrt(e)
	void*  string, *last;
	size_t allocated, length;

	static_assert(std::is_same<type, char>::value || std::is_same<type, wchar_t>::value, L"Error type");
	
public:
	using symbol = type;
	using dynamic_string = type*;
	using const_symbol = const type;
	using const_string = const type*;
	
	using iterator = BasicIterator<type>;
	using const_iterator = BasicIterator<const type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
	decltype(auto) length()
	{
		size_t i = 0;
		for (auto&& c : *this)
			if (c) i++
			else
			{
				length = i;
				return;
			}
	}
	decltype(auto) allocate(size_t al)
	{
		if (al > 0)
		{
			if (!string)
				last = string = new type[allocated = al];
			else if (al == allocated); // maybe adding code to do something!
			else
			{
				if (al < allocated);
					length = length > al ? al : length;
				reinterpret_cast<type*&>(last) = reinterpret_cast<type*>(new(string) type[al]) + length;
				allocated = al;
			}
		}
		else free();
	}
public:
	decltype(auto) push_back(type val)
	{
		if (length >= allocated)
			allocate(allocated * mul_alloc + 1);
		else if (length > 0)
			reinterpret_cast<type*&>(last)++;
		*reinterpret_cast<type*>(last) = val;
		*(reinterpret_cast<type*>(last) + 1) = 0;
		length++;
	}
	decltype(auto) pop_back()
	{
		if (length > 0)
		{
			reinterpret_cast<type*&>(last)--;
			length--;
		}
	}
	decltype(auto) insert(size_t place, type val)
	{
		if (place >= allocated)
		{
			length = place;
			allocate(place * mul_alloc + 1);
			*last = val;
		}
		else if (place > length)
			*(last = string + (length = place)) = val;
		else string[place] = val;
	}
	decltype(auto) insert(size_t place, type* val, size_t sz)
	{
		if (place + sz >= allocated)
			allocate((length = place + sz) * mul_alloc + 1);
		memcpy(reinterpret_cast<type*&>(string) + place, val, sz);
		if (place + sz > length)
			last = reinterpret_cast<type*>(string) + length;
	}
	decltype(auto) size()
	{
		return length;
	}
	decltype(auto) copy(BasicString* v)
	{
		v->allocate(allocated);
		if (length)
		{
			v->length = length;
			memcpy(v->string, string, length * sizeof(type));
		}
	}
	decltype(auto) clear()
	{
		last = string;
		length = 0;
	}
	decltype(auto) back()
	{
		return *reinterpret_cast<type*>(last);
	}
	decltype(auto) right()
	{
		return (length > 0 ? *(reinterpret_cast<type*&>(last)--) : reinterpret_cast<type&>(string));
	}
	decltype(auto) capacity()
	{
		return allocated;
	}
	decltype(auto) data()
	{
		return reinterpret_cast<type*>(string);
	}
	decltype(auto) empty()
	{
		return length == 0;
	}
	decltype(auto) resize(size_t sz)
	{
		if ((length = sz) < allocated)
			reinterpret_cast<type*&>(last) = reinterpret_cast<type*>(string) + length;
		else allocate(length);
	}
	decltype(auto) free()
	{
		allocated = length = 0;
		if (string)
			delete[] string;
		last = nullptr;
	}
	decltype(auto) reserve(size_t sz)
	{
		allocate(sz);
	}
	decltype(auto) rate(double val)
	{
		mul_alloc = val;
	}
	decltype(auto) rate()
	{
		return double& (mul_alloc);
	}
	decltype(auto) max_size()
	{
		return (1 << (8 * sizeof(allocated))) / sizeof(type);
	}
	decltype(auto) size_of()
	{
		return sizeof(type);
	}
	decltype(auto) shrink_to_fit()
	{
		if (length < allocated)
			allocate(length);
	}

	decltype(auto) begin()
	{
		return iterator(reinterpret_cast<type*>(string));
	}
	decltype(auto) end()
	{
		return iterator(reinterpret_cast<type*>(last));
	}
	decltype(auto) cbegin() const
	{
		return const_iterator(reinterpret_cast<type*>(string));
	}
	decltype(auto) cend() const
	{
		return const_iterator(reinterpret_cast<type*>(last));
	}
	decltype(auto) rbegin()
	{
		return reverse_iterator(end());
	}
	decltype(auto) rend()
	{
		return reverse_iterator(begin());
	}
	decltype(auto) crbegin() const
	{
		return const_reverse_iterator(cend());
	}
	decltype(auto) crend() const
	{
		return const_reverse_iterator(cbegin());
	}

	decltype(auto) operator()(std::initializer_list<type> v)
	{
		string = nullptr;
		allocate(length = v.size());
		memcpy(string, v.begin(), length * sizeof(type));
	}
	decltype(auto) operator~()
	{
		free();
	}
	decltype(auto) operator+=(type c)
	{
		push_back(c);
	}
	decltype(auto) operator+=(BasicString s)
	{
		insert(length, s.string, s.length);
	}
	decltype(auto) operator+=(BasicString& s)
	{
		insert(length, s.string, s.length);
	}
	decltype(auto) operator+=(const BasicString s) const
	{
		insert(length, s.string, s.length);
	}
	decltype(auto) operator+=(const BasicString& s) const
	{
		insert(length, s.string, s.length);
	}
	decltype(auto) operator[](size_t i) const
	{
		if (i < length)
			return reinterpret_cast<const_symbol*>(string)[i];
		else return const_symbol(0);
	}
	decltype(auto) operator[](size_t i)
	{
		if (i < length)
			return reinterpret_cast<symbol*>(string)[i];
		else return symbol(0);
	}

	BasicString(std::initializer_list<type> v)
	{
		this->operator()(v);
	}
	BasicString()
	{
		last = string = nullptr;
		allocated = length = 0;
	}
	BasicString(size_t sz)
	{
		length = 0;
		string = nullptr;
		allocate(sz);
	}
	BasicString(size_t sz, type* ray)
	{
		length = 0;
		string = nullptr;
		insert(0, ray, sz);
	}
	BasicString(BasicString& v)
	{
		v.copy(this);
	}

	~BasicString()
	{
		free();
	}
};
