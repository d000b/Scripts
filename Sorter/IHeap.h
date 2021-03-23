#pragma once

#include "ISorter.h"

#if defined(HEAPSORT) && HEAPSORT

namespace
{
	template <typename type>
	void  Heapify(type* a, size_t i, size_t n)
	{
		size_t l = 2 * i + 1;
		size_t r = 2 * i + 2;
		size_t largest = i;
		if (l < n && a[l] > a[i])
		{
			largest = l;
		}
		if (r < n && a[r] > a[l])
		{
			largest = r;
		}
		if (largest != i)
		{
			UltimaAPI::swap(&a[i], &a[largest]);
			Heapify(a, largest, n);
		}
	}

	template <typename type>
	void  Heapify(type* a, size_t i, size_t n, bool function(type, type))
	{
		size_t l = 2 * i + 1;
		size_t r = 2 * i + 2;
		size_t largest = i;
		if (l < n && function(a[l], a[i]))
		{
			largest = l;
		}
		if (r < n && function(a[r], a[l]))
		{
			largest = r;
		}
		if (largest != i)
		{
			UltimaAPI::swap(&a[i], &a[largest]);
			Heapify(a, largest, n, function);
		}
	}
}

template <typename type>
void  UltimaAPI::ISorters::HeapSort(type* a, size_t n)
{
	for (size_t i = n / 2 - 1; i != -1; i--)
	{
		::Heapify(a, i, n);
	}
	for (size_t i = n - 1; i > 0; i--)
	{
		swap(&a[0], &a[i]);
		::Heapify(a, 0, i);
	}
}

template <typename type>
void  UltimaAPI::ISorters::HeapSort(type* a, size_t n, bool function(type, type))
{
	for (size_t i = n / 2 - 1; i >= 0; i--)
	{
		::Heapify(a, i, n, function);
	}
	for (size_t i = n - 1; i > 0; i--)
	{
		swap(a[0], a[i]);
		::Heapify(a, 0, i, function);
	}
}

#else
#if defined(EXCEPTIONS) && EXCEPTIONS

template <typename type>
void  UltimaAPI::ISorters::HeapSort(type*, size_t)
{
	throw(ExceptionISorter(L"HeapSort", 0));
}

template <typename type>
void  UltimaAPI::ISorters::HeapSort(type*, size_t, bool(type, type))
{
	throw(ExceptionISorter(L"HeapSort", 1));
}

#else

template <typename type>
void  UltimaAPI::ISorters::HeapSort(type*, size_t)
{
	throw(L"HeapSortException");
}

template <typename type>
void  UltimaAPI::ISorters::HeapSort(type*, size_t, bool(type, type))
{
	throw(L"HeapSortException");
}

#endif
#endif
