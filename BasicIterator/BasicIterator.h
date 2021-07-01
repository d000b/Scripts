#pragma once

#include <iterator>

namespace UltimaAPI
{
	template <typename type>  class Vector;

	template <typename type, typename maps>  class PseudoTensor;

	template <typename type>  class BasicIterator;
}

template <typename type>
class  UltimaAPI::BasicIterator
{
	extern friend class UltimaAPI::Vector<type>;
	
	using value_type = type;
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using pointer = value_type*;
	using reference = value_type&;

	pointer ptr;
public:
	BasicIterator(pointer p) : ptr(p) { }
	BasicIterator(const BasicIterator& it) : ptr(it.ptr) { }

	bool operator!=(const value_type v) const
	{
		return *ptr != v;
	}
	bool operator!=(const pointer v) const
	{
		return ptr != v;
	}
	bool operator!=(BasicIterator const& it) const
	{
		return ptr != it.ptr;
	}
	bool operator==(const value_type v) const
	{
		return *ptr == v;
	}
	bool operator==(const pointer v) const
	{
		return ptr == v;
	}
	bool operator==(BasicIterator const& it) const
	{
		return ptr == it.ptr;
	}
	reference operator*() const
	{
		return *ptr;
	}
	BasicIterator& operator++()
	{
		ptr++;
		return *this;
	}
	BasicIterator& operator--()
	{
		ptr--;
		return *this;
	}
};
