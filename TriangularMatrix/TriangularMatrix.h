#pragma once;

#include <memory.h>
#include <initializer_list>

#include "../Exception/Exception.h"

namespace UltimaAPI
{
	template <typename type>  class TriangularMatrix;
}

template <typename type>
class  UltimaAPI::TriangularMatrix
{
public:
	enum class flags : __int32
	{
		is_init =     1 << 1,
		is_directed = 1 << 2, // square mem or triangle
		is_diagonal = 1 << 3, // else without main diagonal

		is_default = is_init | is_diagonal | is_directed,

		none =          0 << 1,
		none_directed = 0 << 2,
		none_diagonal = 0 << 3
	};

private:
	flags  bits;
	size_t width;
	size_t allocated;
	void*  start;

	decltype(auto) allocate(size_t al)
	{
		if (al > 0)
		{
			if (!start)
			{
				start = new type[allocated = al];
			}
			else if (al > allocated)
			{
				// need to fix: example in Vector
				new(start) type[allocated = al];
			}
		}
		else free();
	}
public:
	decltype(auto) free()
	{
		allocated = width = 0;
		if (start)
			delete[] start;
		start = nullptr;
	}

	decltype(auto) operator[](size_t i)
	{
		reinterpret_cast<type*&>(start)[i * width + i * (1 - i) / 2];
	}

	decltype(auto) operator()(size_t i)
	{
		return reinterpret_cast<type*&>(start)[i];
	}
	decltype(auto) operator()(size_t i, size_t j)
	{
		// todo fix correcting!
		return reinterpret_cast<type*&>(start)[(width - i) * width + j];
	}

	TriangularMatrix() : bits(flags::none), allocated(0u), width(0u), start(nullptr)
	{
		
	}
	TriangularMatrix(size_t w, flags fl = flags::is_default) : width(w), bits(fl)
	{
		// проработать отдельный случай, где не учавствует главная диагональ ВООБЩЕ (не только в треугольной матрице)
		if (fl & flags::is_directed)
		{
			allocate(allocated = width * width);
		}
		else
		{
			if (fl & flags::is_diagonal)
			{
				allocate(allocated = width * (width + 1) / 2);
			}
			else
			{
				allocate(allocated = width * (width - 1) / 2);
			}
		}
	}
	TriangularMatrix(const TriangularMatrix& m) : width(m.width), allocated(m.allocated), bits(m.bits)
	{
		if ((width * (width + 1) / 2) != allocated)
			throw(Exception(L"shit happense"));
		start = new type[allocated];
		memcpy(start, m.start, sizeof(type) * allocated);
	}
};
