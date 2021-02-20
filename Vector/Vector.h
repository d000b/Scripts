#pragma once;

#include <memory.h>
#include <initializer_list>

namespace UltimaAPI
{
#define MULTIVECTOR 0

	template <typename type>  class VectorIterator;
	template <typename type>  class Vector;

#if defined(MULTIVECTOR) && MULTIVECTOR
	template <typename type>  class MultidimensionalVectorIterator;
	template <typename type>  class MultidimensionalVector;
#endif
}

template <typename type>
class  UltimaAPI::VectorIterator : public std::iterator<std::input_iterator_tag, type>
{
	friend class UltimaAPI::Vector<type>;
#if defined(MULTIVECTOR) && MULTIVECTOR
	friend class UltimaAPI::MultidimensionalVector<type>;
#endif

	type* pointer;
private:
	VectorIterator(type* p) : pointer(p)
	{

	}
public:
	VectorIterator(const VectorIterator& it) : pointer(it.p)
	{

	}

	bool operator!=(VectorIterator const& it) const
	{
		return pointer != it.pointer;
	}
	bool operator==(VectorIterator const& it) const
	{
		return pointer == it.pointer;
	}
	typename VectorIterator::reference operator*() const
	{
		return *pointer;
	}
	VectorIterator& operator++()
	{
		pointer++;
		return *this;
	}
	VectorIterator& operator--()
	{
		pointer--;
		return *this;
	}
};

template <typename type>
class  UltimaAPI::Vector
{
	double mul_alloc = 1.6487; // sqrt(e)

	size_t used;
	size_t allocated;
	void* start, *last;
public:
	using iterator = VectorIterator<type>;
	using const_iterator = VectorIterator<const type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
	decltype(auto) allocate(size_t al)
	{
		if (al > 0)
		{
			if (!start)
			{
				start = new type[allocated = al];
			}
			else if (al == allocated); // maybe adding code to do something!
			else
			{
				if (al > allocated);
				else
				{
					used = used > al ? al : used;
				}
				reinterpret_cast<type*&>(last) = reinterpret_cast<type*>(start = new(start) type[al]) + used;
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
		{
			used = place + sz;
			allocate(used * mul_alloc + 1);
		}
		memcpy(reinterpret_cast<type*&>(start) + place, val, sz);
		if (place + sz > used)
			last = reinterpret_cast<type*>(start) + used;
	}
	decltype(auto) size()
	{
		return used;
	}
	decltype(auto) copy(Vector* v)
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
	decltype(auto) operator[](size_t i)
	{
		if (i >= allocated)
			allocate(i * mul_alloc + 1);
		if (i >= used) used = i + 1;
		return reinterpret_cast<type*&>(start)[i];
	}

	Vector(std::initializer_list<type> v)
	{
		this->operator()(v);
	}
	Vector()
	{
		used = 0;
		start = 0;
		allocate(0);
	}
	Vector(size_t sz)
	{
		used = 0;
		start = nullptr;
		allocate(sz);
	}
	Vector(size_t sz, type* ray)
	{
		used = 0;
		start = nullptr;
		insert(0, ray, sz);
	}
	Vector(Vector& v)
	{
		v.copy(this);
	}

	~Vector()
	{
		free();
	}
};

#if defined(MULTIVECTOR) && MULTIVECTOR
template <typename type>
class UltimaAPI::MultidimensionalVectorIterator : public std::iterator<std::input_iterator_tag, MultidimensionalVector<type>>
{
	friend class NNUltimaAPI::MultidimensionalVector<type>;

	MultidimensionalVector<type>* pointer;

private:
	MultidimensionalVectorIterator(MultidimensionalVector<type>* p) : pointer(p)
	{

	}
public:
	MultidimensionalVectorIterator(const MultidimensionalVectorIterator& it) : pointer(it.p)
	{

	}

	bool operator!=(MultidimensionalVectorIterator const& it) const
	{
		return pointer != it.pointer;
	}
	bool operator==(MultidimensionalVectorIterator const& it) const
	{
		return pointer == it.pointer;
	}
	typename MultidimensionalVectorIterator::reference operator*() const
	{
		return *pointer;
	}
	MultidimensionalVectorIterator& operator++()
	{
		pointer++;
		return *this;
	}
	MultidimensionalVectorIterator& operator--()
	{
		pointer--;
		return *this;
	}
};

template <typename type>
class UltimaAPI::MultidimensionalVector
{
	static_assert(0, "MultidimensionalVector <- FIX ME!");

	double mul_alloc = 1.6487; // sqrt(e)
	enum class Config cfg;
	size_t used;
	size_t allocated;

#define USING_LAST_PTR 1
	// MultidimensionalVector<type>* OR type*
	void* start;
#if defined (USING_LAST_PTR) && (USING_LAST_PTR)
	void* last;
#endif
public:
	enum class Config : size_t
	{
		is_type = 1 << 1, // is ptr used for type
	};
public:
	using iterator = VectorIterator<type>;
	using const_iterator = VectorIterator<const type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	using iterator_vector = MultidimensionalVectorIterator<MultidimensionalVector<type>>;
	using const_iterator_vector = MultidimensionalVectorIterator<const MultidimensionalVector<type>>;
	using reverse_iterator_vector = std::reverse_iterator<iterator_vector>;
	using const_reverse_iterator_vector = std::reverse_iterator<const_iterator_vector>;
private:
	decltype(auto) allocate(size_t al)
	{
		void* ptr = new type[al];
		if (start)
		{
			memcpy(ptr, start, allocated * sizeof(type));
			delete[] start;
		}
		if (last)
		{
			delete[] last;
		}
		reinterpret_cast<type*&>(last) = reinterpret_cast<type*>(start = ptr) + used;
	}
	decltype(auto) allocate(void* p, size_t count, size_t al)
	{
		allocated = al;
		void* ptr = new type[al];
		if (start)
		{
			memcpy(ptr, start, allocated * sizeof(type));
			delete[] start;
		}
		reinterpret_cast<type*&>(last) = reinterpret_cast<type*>(start = ptr) + used;
	}
public:
	decltype(auto) push_back(type val)
	{
		if (used >= allocated)
			allocate(mul_alloc * allocated);
		else reinterpret_cast<type*&>(last)++;
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
			allocate(mul_alloc * place);
		}
		else if (place > used)
		{
			*(last = start + (used = place)) = val;
		}
		else start[place] = val;
	}
	decltype(auto) size()
	{
		return used;
	}
	decltype(auto) clear()
	{
		last = start;
		used = 0;
	}
	decltype(auto) back()
	{
		return *last;
	}
	decltype(auto) capacity()
	{
		return allocated;
	}
	decltype(auto) data()
	{
		return start;
	}
	decltype(auto) empty()
	{
		return used == 0;
	}
	decltype(auto) resize(size_t sz)
	{
		used = sz;
		if (sz < allocated)
			reinterpret_cast<type*&>(last) = reinterpret_cast<type*>(start) + used;
		else  allocate(used);
	}
	decltype(auto) reserve(size_t sz)
	{
		allocate(sz);
	}
	decltype(auto) max_size()
	{
		return (1 << (8 * sizeof(allocated))) / sizeof(type);
	}
	decltype(auto) shrink_to_fit()
	{
		if (used < allocated)
			allocate(used);
	}

	decltype(auto) begin()
	{
		if (cfg & Config::is_type)
			return iterator(reinterpret_cast<type*>(start));
		else return iterator_vector(reinterpret_cast<MultidimensionalVector<type>*>(start));
	}
	decltype(auto) end()
	{
#if defined (USING_LAST_PTR) && (USING_LAST_PTR)
		if (cfg & Config::is_type)
			return iterator(reinterpret_cast<type*>(last));
		else return iterator_vector(reinterpret_cast<MultidimensionalVector<type>*>(last));
#else
		if (cfg & Config::is_type)
			return iterator(reinterpret_cast<type*>(start + used * sizeof(type)));
		else return iterator_vector(reinterpret_cast<MultidimensionalVector<type>*>(start + used * sizeof(type)));
#endif
	}
	decltype(auto) cbegin() const
	{
		if (cfg & Config::is_type)
			return const_iterator(reinterpret_cast<const type*>(start));
		else return const_iterator_vector(reinterpret_cast<MultidimensionalVector<const type>*>(start));
	}
	decltype(auto) cend() const
	{
#if defined (USING_LAST_PTR) && (USING_LAST_PTR)
		if (cfg & Config::is_type)
			return const_iterator(reinterpret_cast<const type*>(last));
		else return const_iterator_vector(reinterpret_cast<MultidimensionalVector<const type>*>(last));
#else
		if (cfg & Config::is_type)
			return const_iterator(reinterpret_cast<const type*>(start + used * sizeof(type)));
		else return const_iterator_vector(reinterpret_cast<MultidimensionalVector<const type>*>(start + used * sizeof(type)));
#endif
	}
	decltype(auto) rbegin()
	{
		if (cfg & Config::is_type)
			return reverse_iterator(end());
		else return reverse_iterator_vector(end());
	}
	decltype(auto) rend()
	{
		if (cfg & Config::is_type)
			return reverse_iterator(begin());
		else return reverse_iterator_vector(begin());
	}
	decltype(auto) crbegin() const
	{
		if (cfg & Config::is_type)
			return const_reverse_iterator(cend());
		else return const_reverse_iterator_vector(cend());
	}
	decltype(auto) crend() const
	{
		if (cfg & Config::is_type)
			return reverse_iterator(cbegin());
		else return reverse_iterator_vector(cbegin());
	}

	decltype(auto) operator[](size_t i)
	{
		if (i < allocated)
		{
			if (cfg & Config::is_type)
				return reinterpret_cast<type*>(start)[i];
			else return reinterpret_cast<MultidimensionalVector<type*>>(start)[i];
		}
		else
		{
			if (cfg & Config::is_type)
				return *reinterpret_cast<type*>(start);
			else return *reinterpret_cast<MultidimensionalVector<type*>>(start);
		}
	}

	MultidimensionalVector(std::initializer_list<type> v)
	{
		allocate(used = v.size());
		memcpy(start, v.begin(), used * sizeof(type));
	}
	MultidimensionalVector()
	{
		start = 0;
		allocate(4);
	}
	MultidimensionalVector(size_t sz)
	{
		start = 0;
		allocate(sz);
	}

	~MultidimensionalVector()
	{
		if (allocated)
			delete[] start;
	}
};
#endif
