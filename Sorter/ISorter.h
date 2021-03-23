#pragma once

#define EXCEPTIONS 1

#define HEAPSORT         1
#define INSERTIONSORT    1
#define BINARYSORTEDTREE 1
#define MERGESORT        1
#define QUICKSORT        1
#define SELECTIONSORT    1

#if defined(EXCEPTIONS) && EXCEPTIONS
#include "../Exception/Exception.h"
#endif

namespace UltimaAPI
{
	template <typename type>
	void swap(type* a, type* b)
	{
		auto _ = *a;
		*a = *b;
		*b = _;
	}

	template <typename type>
	int ForwardSort(type a, type b)
	{
		return (a < b) - (b < a);
	}
	template <typename type>
	int BackwardSort(type a, type b)
	{
		return (b < a) - (a < b);
	}

	class ISorters
	{
	public:
#if defined(EXCEPTIONS) && EXCEPTIONS
		class ExceptionISorter;
#endif

		template <typename type>  class IBinaryTree;

		template <typename type>  void  HeapSort(type*, size_t);
		template <typename type>  void  InsertionSort(type*, size_t);
		template <typename type>  bool  BinarySortedTree(IBinaryTree<type>*); // TODO
		template <typename type>  void  MergeSort(type*, size_t);
		template <typename type>  void  QuickSort(type*, size_t, size_t);
		template <typename type>  void  SelectionSort(type*, size_t);

		template <typename type>  void  HeapSort(type*, size_t, int(type, type));
		template <typename type>  void  InsertionSort(type*, size_t, int(type, type));
		template <typename type>  bool  BinarySortedTree(IBinaryTree<type>*, int(type, type)); // TODO
		template <typename type>  void  MergeSort(type*, size_t, int(type, type));
		template <typename type>  void  QuickSort(type*, size_t, size_t, int(type, type)); // TODO
		template <typename type>  void  SelectionSort(type*, size_t, int(type, type)); // TODO
	};
}

#if defined(EXCEPTIONS) && EXCEPTIONS
class UltimaAPI::ISorters::ExceptionISorter : public UltimaAPI::Exception
{
	ExceptionISorter(const ExceptionISorter& ex) : Exception(ex) { }
public:
	ExceptionISorter(const wchar_t* m) : Exception(m) { }
	ExceptionISorter(const wchar_t* m, __int64 c) : Exception(m, c) { }
};
#endif

#if defined(какая_то_хуета)
int*  quick_power(int* a, int n) 
{
	int* result = I;
	while (n)
	{
		if (n & 1)
		{
			result = multi(result, a);
		}
		n >>= 1;
		a = multi(a, a);
	}
	return result;
}
#endif
