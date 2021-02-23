#pragma once;

#include <memory.h>
#include <iterator>
#include <initializer_list>

namespace UltimaAPI
{
	template <typename type>  class CircularVector;
	template <typename type>  class CircularVectorIterator;
}

// this is pasta from Vector, I need to sleep!
template <typename type>
class  UltimaAPI::CircularVector
{
	double mul_alloc = 1.6487; // sqrt(e)

	size_t used;
	size_t allocated;
	void* start, *current, *last;
public:
	using iterator = CircularVectorIterator<type>;
	using const_iterator = CircularVectorIterator<const type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
	decltype(auto) allocate(size_t al)
	{
		if (al > 0)
		{
			if (!start)
			{
				last = start = new type[allocated = al];
			}
			else if (al == allocated); // maybe adding code to do something!
			else
			{
				if (al < allocated);
				{
					used = used > al ? al : used;
				}
				reinterpret_cast<type*&>(last) = reinterpret_cast<type*>(new(start) type[al]) + used;
				allocated = al;
			}
		}
		else free();
	}
public:
	decltype(auto) push_back(type val)
	{
		if (used >= allocated)
			allocate(allocated * mul_alloc + 1);
		else if (used > 0)
			reinterpret_cast<type*&>(last)++;
		*reinterpret_cast<type*>(last) = val;
		used++;
	}
	decltype(auto) pop_back()
	{
		if (used > 0)
		{
			reinterpret_cast<type*&>(last)--;
			used--;
		}
	}
	decltype(auto) insert(size_t place, type val)
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
	decltype(auto) insert(size_t place, type* val, size_t sz)
	{
		if (place + sz >= allocated)
			allocate((used = place + sz) * mul_alloc + 1);
		memcpy(reinterpret_cast<type*&>(start) + place, val, sz);
		if (place + sz > used)
			last = reinterpret_cast<type*>(start) + used;
	}
	decltype(auto) size()
	{
		return used;
	}
	decltype(auto) copy(CircularVector* v)
	{
		v->allocate(allocated);
		if (used)
		{
			v->used = used;
			memcpy(v->start, start, used * sizeof(type));
		}
	}
	decltype(auto) clear()
	{
		last = start;
		used = 0;
	}
	decltype(auto) back()
	{
		return *reinterpret_cast<type*>(last);
	}
	decltype(auto) right() // not safe func! After using check allocated size (used > 0)
	{
		return (used > 0 ? *(reinterpret_cast<type*&>(last)--) : *(reinterpret_cast<type*&>(start)));
	}
	decltype(auto) capacity()
	{
		return allocated;
	}
	decltype(auto) data()
	{
		return reinterpret_cast<type*>(start);
	}
	decltype(auto) empty()
	{
		return used == 0;
	}
	decltype(auto) resize(size_t sz)
	{
		if ((used = sz) < allocated)
			reinterpret_cast<type*&>(last) = reinterpret_cast<type*>(start) + used;
		else allocate(used);
	}
	decltype(auto) free()
	{
		allocated = used = 0;
		if (start)
			delete[] start;
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
		return iterator(reinterpret_cast<type*>(start));
	}
	decltype(auto) end()
	{
		return iterator(reinterpret_cast<type*>(last));
	}
	decltype(auto) cbegin() const
	{
		return const_iterator(reinterpret_cast<type*>(start));
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
		start = nullptr;
		allocate(used = v.size());
		memcpy(start, v.begin(), used * sizeof(type));
	}
	decltype(auto) operator~()
	{
		free();
	}
	decltype(auto) operator+=(type c)
	{
		push_back(c);
	}
	decltype(auto) operator+=(CircularVector v)
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator+=(CircularVector& v)
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator+=(const CircularVector v) const
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator+=(const CircularVector& v) const
	{
		insert(used, v.start, v.used);
	}
	decltype(auto) operator[](size_t i)
	{
		if (i >= allocated)
			allocate(i * mul_alloc + 1);
		if (i >= used) used = i + 1;
		return reinterpret_cast<type*&>(start)[i];
	}

	CircularVector(std::initializer_list<type> v)
	{
		this->operator()(v);
	}
	CircularVector()
	{
		last = start = nullptr;
		allocated = used = 0;
	}
	CircularVector(size_t sz)
	{
		used = 0;
		start = nullptr;
		allocate(sz);
	}
	CircularVector(size_t sz, type* ray)
	{
		used = 0;
		start = nullptr;
		insert(0, ray, sz);
	}
	CircularVector(CircularVector& v)
	{
		v.copy(this);
	}

	~CircularVector()
	{
		free();
	}
};

template <typename type>
class  UltimaAPI::CircularVectorIterator : public std::iterator<std::input_iterator_tag, type>
{
	friend class UltimaAPI::CircularVector<type>;

	type* pointer;
private:
	CircularVectorIterator(type* p) : pointer(p)
	{

	}
public:
	CircularVectorIterator(const CircularVectorIterator& it) : pointer(it.p)
	{

	}

	bool operator!=(CircularVectorIterator const& it) const
	{
		return pointer != it.pointer;
	}
	bool operator==(CircularVectorIterator const& it) const
	{
		return pointer == it.pointer;
	}
	typename CircularVectorIterator::reference operator*() const
	{
		return *pointer;
	}
	CircularVectorIterator& operator++()
	{
		pointer++;
		return *this;
	}
	CircularVectorIterator& operator--()
	{
		pointer--;
		return *this;
	}
};
