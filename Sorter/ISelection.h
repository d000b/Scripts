#pragma once

#include "ISorter.h"

#if defined(SELECTIONSORT) && SELECTIONSORT

template <typename type>
void  UltimaAPI::ISorters::SelectionSort(type* s, size_t n)
{
	for (size_t i = 0; i < n - 1; i++)
	{
		size_t min = i;
		for (size_t j = i + 1; j < n; j++)
			if (s[j] < s[min])
			{
				min = j;
			}
		swap(&s[i], &s[min]);
	}
}

template <typename type>
void  UltimaAPI::ISorters::SelectionSort(type* s, size_t n, int (*function)(type, type))
{
	for (size_t i = 0; i < n - 1; i++)
	{
		size_t min = i;
		for (size_t j = i + 1; j < n; j++)
			if (function(s[j], s[min]) > 0)
			{
				min = j;
			}
		swap(&s[i], &s[min]);
	}
}

#else
#if defined(EXCEPTIONS) && EXCEPTIONS

template <typename type>
void  UltimaAPI::ISorters::SelectionSort(type*, size_t)
{
	throw(ExceptionISorter(L"SelectionSort", 0));
}

template <typename type>
void  UltimaAPI::ISorters::SelectionSort(type*, size_t, bool(type, type))
{
	throw(ExceptionISorter(L"SelectionSort", 1));
}

#else

template <typename type>
void  UltimaAPI::ISorters::SelectionSort(type*, size_t)
{
	throw(L"SelectionSortException");
}

template <typename type>
void  UltimaAPI::ISorters::SelectionSort(type*, size_t, bool(type, type))
{
	throw(L"SelectionSortException");
}

#endif
#endif
