#pragma once;

#include <iterator>

namespace UltimaAPI
{
	template <typename type>
	class Vector;
	template <typename type>
	class BasicString;

	template <typename type>
	class BasicIterator;
}

template <typename type>
class  UltimaAPI::BasicIterator : public std::iterator<std::input_iterator_tag, type>
{
	extern friend class UltimaAPI::Vector<type>;
	extern friend class UltimaAPI::BasicString<type>;

	type* pointer;
private:
	BasicIterator(type* p) : pointer(p)
	{

	}
public:
	BasicIterator(const BasicIterator& it) : pointer(it.p)
	{

	}

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