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


template <typename type>
class  UltimaAPI::BasicString
{
	double mul_alloc = 1.6487; // sqrt(e)
	void*  string, *last;
	size_t allocated, used;

	static_assert(std::is_same<type, char>::value || std::is_same<type, wchar_t>::value, L"Error type");
	
	static_assert(false, L"fix length error");
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
			if (c) i++;
			else
			{
				used = i;
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
					used = used > al ? al : used;
				reinterpret_cast<type*&>(last) = reinterpret_cast<type*>(new(string) type[al]) + used;
				allocated = al;
			}
		}
		else free();
	}
public:
	decltype(auto) push_back(type val)
	{
		if (used + 1 >= allocated)
			allocate(allocated * mul_alloc + 1);
		else if (used > 0)
			reinterpret_cast<type*&>(last)++;
		*reinterpret_cast<type*>(last) = val;
		*(reinterpret_cast<type*>(last)++) = 0;
		used++;
	}
	decltype(auto) pop_back()
	{
		if (used > 0)
		{
			*(reinterpret_cast<type*&>(last)--) = 0;
			used--;
		}
	}
	decltype(auto) insert(size_t place, type val)
	{
		if (place + 1 >= allocated)
		{
			used = place;
			allocate(place * mul_alloc + 1);
			*last = val;
			*(reinterpret_cast<type*&>(last)++) = 0;
		}
		else if (place > used)
		{
			*(last = string + (used = place)) = val;
			*(reinterpret_cast<type*&>(last)++) = 0;
		}
		else string[place] = val;
	}
	decltype(auto) insert(size_t place, type* val, size_t sz)
	{
		if (place + sz >= allocated)
			allocate((used = place + sz) * mul_alloc + 1);
		memcpy(reinterpret_cast<type*&>(string) + place, val, sz);
		if (place + sz > used)
			last = reinterpret_cast<type*>(string) + used;
	}
	decltype(auto) size()
	{
		return used;
	}
	decltype(auto) copy(BasicString* v)
	{
		v->allocate(allocated);
		if (used)
		{
			v->used = used;
			memcpy(v->string, string, used * sizeof(type));
		}
	}
	decltype(auto) clear()
	{
		last = string;
		used = 0;
	}
	decltype(auto) back()
	{
		return *reinterpret_cast<type*>(last);
	}
	decltype(auto) right()
	{
		return (used > 0 ? *(reinterpret_cast<type*&>(last)--) : reinterpret_cast<type&>(string));
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
		return used == 0;
	}
	decltype(auto) resize(size_t sz)
	{
		if ((used = sz) < allocated)
			reinterpret_cast<type*&>(last) = reinterpret_cast<type*>(string) + used;
		else allocate(used);
	}
	decltype(auto) free()
	{
		allocated = used = 0;
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
		if (used < allocated)
			allocate(used);
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
		allocate(used = v.size());
		memcpy(string, v.begin(), used * sizeof(type));
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
		insert(used, s.string, s.used);
	}
	decltype(auto) operator+=(BasicString& s)
	{
		insert(used, s.string, s.used);
	}
	decltype(auto) operator+=(const BasicString s) const
	{
		insert(used, s.string, s.used);
	}
	decltype(auto) operator+=(const BasicString& s) const
	{
		insert(used, s.string, s.used);
	}
	decltype(auto) operator[](size_t i) const
	{
		if (i < used)
			return reinterpret_cast<const_symbol*>(string)[i];
		else return const_symbol(0);
	}
	decltype(auto) operator[](size_t i)
	{
		if (i < used)
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
		allocated = used = 0;
	}
	BasicString(size_t sz)
	{
		used = 0;
		string = nullptr;
		allocate(sz);
	}
	BasicString(size_t sz, type* ray)
	{
		used = 0;
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
