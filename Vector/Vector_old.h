#pragma once

#include <memory.h>
#include <initializer_list>

#include "../BasicIterator/BasicIterator.h"
#include "../Memory/util.h"

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
	type*  start;
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
				start = new type[allocated = al];
			else if (al == allocated); // maybe adding code to do something!
			else
			{
				type* block;
				memcpy(block = new type[allocated = al], start, (used = used > al ? al : used) * sizeof(type));
				delete[] start;
				start = block;
			}
		}
		else free();
	}
public:
	decltype(auto) push_back(type val) noexcept
	{
		if (used >= allocated)
			allocate(alloc_step());
		start[used] = val;
		++used;
	}
	decltype(auto) pop_back() noexcept
	{
		if (used > 0)
			--used;
	}
	decltype(auto) insert_correct(size_t place)
	{
		bool ret = true;
		if (place > used)
		{
			used = place;
			ret = false;
		}
		if (used >= allocated)
			allocate(used * mul_alloc + 1);
		return ret;
	}
	decltype(auto) insert_correct(size_t place, size_t count)
	{
		bool ret = true;
		if (place + count > used)
		{
			used = place + count;
			ret = false;
		}
		if (used >= allocated)
			allocate(used * mul_alloc + 1);
		return ret;
	}
	decltype(auto) move_insert(size_t place, type val)
	{
		if (insert_correct(place))
			Memory::memcpy(start + place + 1, start + place, used - place);
		start[place] = val;
	}
	decltype(auto) move_insert(size_t place, type* val, size_t count)
	{
		if (insert_correct(place, count))
			Memory::memcpy(start + place + count, start + place, used - place);
		Memory::memcpy(start + place, val, count * sizeof(type));
	}
	decltype(auto) insert(size_t place, type val) noexcept
	{
		insert_correct(place);
		start[place] = val;
	}
	decltype(auto) insert(size_t place, type* val, size_t count) noexcept
	{
		insert_correct(place, count);
		memcpy(start + place, val, count * sizeof(type));
	}
	decltype(auto) size() noexcept
	{
		return used;
	}
	decltype(auto) size() const noexcept
	{
		return used;
	}
	decltype(auto) capacity() noexcept
	{
		return allocated;
	}
	decltype(auto) capacity() const noexcept
	{
		return allocated;
	}
	decltype(auto) alloc_step()
	{
		return size_t(allocated * mul_alloc + 1);
	}
	decltype(auto) copy(Vector<type>* v) noexcept
	{
		v->allocate(allocated);
		if (used)
			memcpy(v->start, start, (v->used = used) * sizeof(type));
	}
	decltype(auto) copy(Vector<type>* v) const noexcept
	{
		v->allocate(allocated);
		if (used)
			memcpy(v->start, start, (v->used = used) * sizeof(type));
	}
	decltype(auto) clear() noexcept
	{
		used = 0;
	}
	decltype(auto) clear() const noexcept
	{
		used = 0;
	}
	decltype(auto) erase(size_t i) noexcept
	{
		start[i] = 0; // ??
		// memcpy(start + i, )
	}
	decltype(auto) last() noexcept
	{
		return start + used;
	}
	decltype(auto) last() const noexcept
	{
		return start + used;
	}
	decltype(auto) back() noexcept
	{
		return *(start + used);
	}
	decltype(auto) back() const noexcept
	{
		return *(start + used);
	}
	decltype(auto) data() noexcept
	{
		return start;
	}
	decltype(auto) data() const noexcept
	{
		return start;
	}
	decltype(auto) swap(size_t i, size_t j)
	{
		if (i == j)
			return;
		insert_correct(i > j ? i : j);
		std::swap(start[i], start[j]);
	}
	decltype(auto) swap(Vector<type>& v) noexcept
	{
		std::swap(*this, v);
	}
	decltype(auto) swap(Vector<type>& v) const noexcept
	{
		std::swap(*this, v);
	}
	decltype(auto) empty() noexcept
	{
		return used == 0;
	}
	decltype(auto) empty() const noexcept
	{
		return used == 0;
	}
	decltype(auto) resize(size_t sz) noexcept
	{
		if ((used = sz) > allocated)
			allocate(used);
	}
	decltype(auto) free() noexcept
	{
		allocated = used = 0;
		if (start)
			delete[] start;
		start = nullptr;
	}
	decltype(auto) free() const noexcept
	{
		allocated = used = 0;
		if (start)
			delete[] start;
		start = nullptr;
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
		return iterator(last());
	}
	decltype(auto) cbegin() const noexcept
	{
		return const_iterator(start);
	}
	decltype(auto) cend() const noexcept
	{
		return const_iterator(last());
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
	decltype(auto) operator+=(const Vector<type> v) const  noexcept
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator+=(const Vector<type>& v) const noexcept
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator+=(const Vector<type>&& v) const noexcept
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator[](size_t i) noexcept
	{
		if (i >= allocated)
			allocate(i * mul_alloc + 1);
		if (i >= used)
			used = i + 1;
		return start[i];
	}
	decltype(auto) operator[](size_t i) const noexcept
	{
		return start[i];
	}

	__inline decltype(auto) operator<(size_t i) const noexcept
	{
		return used < i;
	}
	__inline decltype(auto) operator>(size_t i) const noexcept
	{
		return used > i;
	}
	__inline decltype(auto) operator<=(size_t i) const noexcept
	{
		return used <= i;
	}
	__inline decltype(auto) operator>=(size_t i) const noexcept
	{
		return used >= i;
	}
	
	Vector(std::initializer_list<type> v) noexcept
	{
		this->operator()(v);
	}
	Vector() noexcept
	{
		start = nullptr;
		allocated = used = 0;
	}
	Vector(size_t sz) noexcept
	{
		allocated = used = 0;
		start = nullptr;
		allocate(sz);
	}
	Vector(size_t sz, type* ray) noexcept
	{
		allocated = used = 0;
		start = nullptr;
		insert(0, ray, sz);
	}
	Vector(Vector<type>& v) noexcept
	{
		v.copy(this);
	}

	Vector(const size_t sz, const type* ray) noexcept
	{
		allocated = used = 0;
		start = nullptr;
		insert(0, ray, sz);
	}
	Vector(const Vector<type>& v) noexcept
	{
		v.copy(this);
	}

	~Vector() noexcept
	{
		free();
	}
};
