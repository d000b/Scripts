#pragma once

#include <memory.h>
#include <initializer_list>

#include "../BasicIterator/BasicIterator.h"

namespace UltimaAPI
{
	template <typename type>  class Vector;
}

template <typename type>
class UltimaAPI::Vector
{
	double mul_alloc = 1.6487; // sqrt(e)

	size_t used;
	size_t allocated;
	type* start, *last;
public:
	using iterator = BasicIterator<type>;
	using const_iterator = BasicIterator<const type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
	decltype(auto) allocate(size_t al) noexcept
	{
		if (al)
		{
			if (!start)
			{
				last = start = new type[allocated = al];
			}
			else if (al == allocated); // maybe adding code to do something!
			else
			{
				void* block;
				if (al < allocated)
				{
					used = used > al ? al : used;
				}
				memcpy(block = new type[allocated = al], start, used * sizeof(type));
				delete[] start;
				last = (start = block) + used;
			}
		}
		else free();
	}
public:
	decltype(auto) push_back(type val) noexcept
	{
		if (used >= allocated)
			allocate(allocated * mul_alloc + 1);
		else if (used > 0)
			++last;
		*last = val;
		++used;
	}
	decltype(auto) pop_back() noexcept
	{
		if (used > 0)
		{
			--last;
			--used;
		}
	}
	decltype(auto) insert(size_t place, type val) noexcept
	{
		if (place >= allocated)
		{
			used = place;
			allocate(place * mul_alloc + 1);
			*last = val;
		}
		else if (place > used)
		{
			*(last = start + (used = place)) = val;
		}
		else start[place] = val;
	}
	decltype(auto) insert(size_t place, type* val, size_t sz) noexcept
	{
		if (place + sz >= allocated)
			allocate((used = place + sz) * mul_alloc + 1);
		memcpy(start + place, val, sz * sizeof(type));
		if (place + sz > used)
			last = start + used;
	}
	decltype(auto) size() noexcept
	{
		return used;
	}
	decltype(auto) copy(Vector* v) noexcept
	{
		v->allocate(allocated);
		if (used)
		{
			v->used = used;
			memcpy(v->start, start, used * sizeof(type));
		}
	}
	decltype(auto) clear() noexcept
	{
		last = start;
		used = 0;
	}
	decltype(auto) back() noexcept
	{
		return *last;
	}
	decltype(auto) capacity() noexcept
	{
		return allocated;
	}
	decltype(auto) data() noexcept
	{
		return start;
	}
	decltype(auto) swap(Vector& v) noexcept
	{
		std::swap(*this, v);
	}
	decltype(auto) empty() noexcept
	{
		return used == 0;
	}
	decltype(auto) resize(size_t sz) noexcept
	{
		if ((used = sz) <= allocated)
			last = start + used;
		else allocate(used);
	}
	decltype(auto) free() noexcept
	{
		allocated = used = 0;
		if (start)
			delete[] start;
		last = start = nullptr;
	}
	decltype(auto) reserve(size_t sz) noexcept
	{
		allocate(sz);
	}
	decltype(auto) rate(double val) noexcept
	{
		return double&(mul_alloc = val);
	}
	decltype(auto) rate() noexcept
	{
		return double&(mul_alloc);
	}
	decltype(auto) max_size() noexcept
	{
		return (1 << (8 * sizeof(allocated))) / sizeof(type);
	}
	decltype(auto) size_of() noexcept
	{
		return sizeof(type);
	}
	decltype(auto) shrink_to_fit() noexcept
	{
		if (used < allocated)
			allocate(used);
	}

	decltype(auto) begin() noexcept
	{
		return iterator(start);
	}
	decltype(auto) end() noexcept
	{
		return iterator(start + used);
	}
	decltype(auto) cbegin() const noexcept
	{
		return const_iterator(start);
	}
	decltype(auto) cend() const noexcept
	{
		return const_iterator(start + used);
	}
	decltype(auto) rbegin() noexcept
	{
		return reverse_iterator(end());
	}
	decltype(auto) rend() noexcept
	{
		return reverse_iterator(begin());
	}
	decltype(auto) crbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}
	decltype(auto) crend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	decltype(auto) operator()(std::initializer_list<type> v) noexcept
	{
		start = nullptr;
		allocate(used = v.size());
		memcpy(start, v.begin(), used * sizeof(type));
	}
	decltype(auto) operator~() noexcept
	{
		free();
	}
	decltype(auto) operator^=(Vector& v) noexcept
	{
		swap(v);
	}
	decltype(auto) operator+=(type c) noexcept
	{
		push_back(c);
	}
	decltype(auto) operator+=(Vector v) noexcept
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator+=(Vector& v) noexcept
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator+=(Vector&& v) noexcept
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator+=(const Vector v) const  noexcept
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator+=(const Vector& v) const noexcept
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator+=(const Vector&& v) const noexcept
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator[](size_t i) noexcept
	{
		if (i >= allocated)
			allocate(i * mul_alloc + 1);
		if (i >= used)
		{
			used = i + 1;
			last = start + used;
		}
		return start[i];
	}

	Vector(std::initializer_list<type> v) noexcept
	{
		this->operator()(v);
	}
	Vector() noexcept
	{
		last = start = nullptr;
		allocated = used = 0;
	}
	Vector(size_t sz) noexcept
	{
		used = 0;
		start = nullptr;
		allocate(sz);
	}
	Vector(size_t sz, type* ray) noexcept
	{
		used = 0;
		start = nullptr;
		insert(0, ray, sz);
	}
	Vector(Vector& v) noexcept
	{
		v.copy(this);
	}

	~Vector() noexcept
	{
		free();
	}
};
