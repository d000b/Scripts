#pragma once

#include <stdlib.h>

#include "ISorter.h"

#if defined(MERGESORT) && MERGESORT

namespace
{
	template <typename type>
	void merge(type* a, type* temp, size_t left, size_t mid, size_t right)
	{
		size_t i = left, j = mid, k = left;
		while (i <= mid - 1 && j <= right)
		{
			if (a[i] <= a[j])
			{
				temp[k++] = a[i++];
			}
			else
			{
				temp[k++] = a[j++];
			}
		}
		while (i <= mid - 1)
		{
			temp[k++] = a[i++];
		}
		while (j <= right)
		{
			temp[k++] = a[j++];
		}
		for (i = left; i <= right; i++)
		{
			a[i] = temp[i];
		}
	}

	template <typename type>
	void mergesort(type* a, type* temp, size_t left, size_t right)
	{
		size_t mid = left + (right - left) / 2;
		if (right > left)
		{
			mergesort(a, temp, left, mid);
			mergesort(a, temp, mid + 1, right);
			merge(a, temp, left, mid + 1, right);
		}
	}

	template <typename type>
	void merge(type* a, type* temp, size_t left, size_t mid, size_t right, bool function(type, type))
	{
		size_t i = left, j = mid, k = left;
		while (i <= mid - 1 && j <= right)
		{
			if (function(a[i], a[j]))
			{
				temp[k++] = a[i++];
			}
			else
			{
				temp[k++] = a[j++];
			}
		}
		while (i <= mid - 1)
		{
			temp[k++] = a[i++];
		}
		while (j <= right)
		{
			temp[k++] = a[j++];
		}
		for (i = left; i <= right; i++)
		{
			a[i] = temp[i];
		}
	}

	template <typename type>
	void mergesort(type* a, type* temp, size_t left, size_t right, bool function(type, type))
	{
		size_t mid = left + (right - left) / 2;
		if (right > left)
		{
			mergesort(a, temp, left, mid, function);
			mergesort(a, temp, mid + 1, right, function);
			merge(a, temp, left, mid + 1, right, function);
		}
	}
}

template <typename type>
void  UltimaAPI::ISorters::MergeSort(type* a, size_t n)
{
	type* temp;
	if (temp = (type*)malloc(n * sizeof(type)))
	{
		::mergesort(a, temp, 0, n - 1);
		free(temp);
	}
}

template <typename type>
void  UltimaAPI::ISorters::MergeSort(type* a, size_t n, bool function(type, type))
{
	// need rewrite ::mergesort (bool function was no works)
	throw("bruh, that's no work, anythink");
	type* temp;
	if (temp = (type*)malloc(n * sizeof(type)))
	{
		::mergesort(a, temp, 0, n - 1, function);
		free(temp);
	}
}

#else
#if defined(EXCEPTIONS) && EXCEPTIONS

template <typename type>
void  UltimaAPI::ISorters::MergeSort(type*, size_t)
{
	throw(ExceptionISorter(L"MergeSort", 0));
}

template <typename type>
void  UltimaAPI::ISorters::MergeSort(type*, size_t, bool(type, type))
{
	throw(ExceptionISorter(L"MergeSort", 1));
}

#else

template <typename type>
void  UltimaAPI::ISorters::MergeSort(type*, size_t)
{
	throw(L"MergeSortException");
}

template <typename type>
void  UltimaAPI::ISorters::MergeSort(type*, size_t, bool(type, type))
{
	throw(L"MergeSortException");
}

#endif
#endif