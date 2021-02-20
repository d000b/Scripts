#pragma once;

#include <memory.h>
#include <initializer_list>

namespace UltimaAPI
{
	template <typename type>
	class SmartPointer;
};

template <typename type>
class  UltimaAPI::SmartPointer
{
	type* p;
	size_t* l;
public:
	decltype(auto) link()
	{
		return l;
	}
	decltype(auto) pointer()
	{
		return p;
	}
	decltype(auto) operator~()
	{
		if (l)
		{
			--*l;
			if (!*l)
				delete p, l;
		}
	}

	SmartPointer()
	{
		p = nullptr;
		l = nullptr;
	}
	SmartPointer(type v)
	{
		p = new type(v);
		l = new size_t(1);
	}
	SmartPointer(SmartPointer& v)
	{
		p = v.p;
		l = v.l;
	}

	~SmartPointer()
	{
		this->operator~;
	}
};
