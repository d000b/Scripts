#pragma once

#include <iterator>

namespace UltimaAPI
{
	template <typename type>  class Vector;

	template <typename type, typename maps>  class PseudoTensor;

	template <typename type>  class BasicIterator;
}

template <typename type>
class  UltimaAPI::BasicIterator : public std::iterator<std::input_iterator_tag, type>
{
	extern friend class UltimaAPI::Vector<type>;

	type* pointer;
public:
	BasicIterator(type* p) : pointer(p) { }
	BasicIterator(const BasicIterator& it) : pointer(it.p) { }

	bool operator!=(BasicIterator const& it) const
	{
		return pointer != it.pointer;
	}
	bool operator==(BasicIterator const& it) const
	{
		return pointer == it.pointer;
	}
	typename BasicIterator::reference operator*() const
	{
		return *pointer;
	}
	BasicIterator& operator++()
	{
		pointer++;
		return *this;
	}
	BasicIterator& operator--()
	{
		pointer--;
		return *this;
	}
};
