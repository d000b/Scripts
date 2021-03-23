#pragma once

#include "ISorter.h"

#if defined(INSERTIONSORT) && INSERTIONSORT

template <typename type>
void  UltimaAPI::ISorters::InsertionSort(type* s, size_t n)
{
	for (size_t i = 0; i < n - 1; i++)
	{
		size_t min = i;
		for (size_t j = i + 1; j < n; j++)
			if (s[j] < s[min])
				swap(&s[i], &s[j]);
	}
}

template <typename type>
void  UltimaAPI::ISorters::InsertionSort(type* s, size_t n, bool function(type, type))
{
	for (size_t i = 0; i < n - 1; i++)
	{
		size_t min = i;
		for (size_t j = i + 1; j < n; j++)
			if (function(s[j], s[min]))
				swap(&s[i], &s[j]);
	}
}

#else
#if defined(EXCEPTIONS) && EXCEPTIONS

template <typename type>
void  UltimaAPI::ISorters::InsertionSort(type*, size_t)
{
	throw(ExceptionISorter(L"InsertionSort", 0));
}

template <typename type>
void  UltimaAPI::ISorters::InsertionSort(type*, size_t, bool(type, type))
{
	throw(ExceptionISorter(L"InsertionSort", 1));
}

#else

template <typename type>
void  UltimaAPI::ISorters::InsertionSort(type*, size_t)
{
	throw(L"InsertionSortException");
}

template <typename type>
void  UltimaAPI::ISorters::InsertionSort(type*, size_t, bool(type, type))
{
	throw(L"InsertionSortException");
}

#endif
#endif