#pragma once

#include "ISorter.h"

#if defined(QUICKSORT) && QUICKSORT

template <typename type>
void UltimaAPI::ISorters::QuickSort(type* s, size_t left, size_t right)
{
	size_t i = left, j = right, 
		pivot = s[(left + right) / 2];
	while (i <= j)
	{
		while (s[i] < pivot)  i++;
		while (s[j] > pivot)  j--;
		if (i <= j)
		{
			std::swap(&s[i], &s[j]);
		}
		i++;
		j--;
	}
	if (left < j)
	{
		quicksort(s, left, j);
	}
	if (right > i)
	{
		quicksort(s, i, right);
	}
}

template <typename type>
void UltimaAPI::ISorters::QuickSort(type* s, size_t left, size_t right, bool function(type, type))
{
	throw(L"TODO");

	size_t i = left, j = right,
		pivot = s[(left + right) / 2];
	while (i <= j)
	{
		while (s[i] < pivot)  i++;
		while (s[j] > pivot)  j--;
		if (i <= j)
		{
			std::swap(&s[i], &s[j]);
		}
		i++;
		j--;
	}
	if (left < j)
	{
		quicksort(s, left, j);
	}
	if (right > i)
	{
		quicksort(s, i, right);
	}
}

#else
#if defined(EXCEPTIONS) && EXCEPTIONS

template <typename type>
void  UltimaAPI::ISorters::QuickSort(type*, size_t, size_t)
{
	throw(ExceptionISorter(L"QuickSort", 0));
}

template <typename type>
void  UltimaAPI::ISorters::QuickSort(type*, size_t, size_t, bool(type, type))
{
	throw(ExceptionISorter(L"QuickSort", 1));
}

#else

template <typename type>
void  UltimaAPI::ISorters::QuickSort(type*, size_t, size_t)
{
	throw(L"QuickSortException");
}

template <typename type>
void  UltimaAPI::ISorters::QuickSort(type*, size_t, size_t, bool(type, type))
{
	throw(L"QuickSortException");
}

#endif
#endif