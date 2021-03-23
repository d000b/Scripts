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
	class ISorters
	{
		template <typename type>
		void swap(type* a, type* b)
		{
			type* _ = *a;
			*a = *b;
			*b = *_;
		}
	public:
#if defined(EXCEPTIONS) && EXCEPTIONS
		class ExceptionISorter;
#endif

		template <typename type>  class IBinaryTree;

		template <typename type>  bool  more(type, type);
		template <typename type>  bool  less(type, type);
		template <typename type>  bool  more_equal(type, type);
		template <typename type>  bool  less_equal(type, type);

		template <typename type>  void  HeapSort(type*, size_t);
		template <typename type>  void  InsertionSort(type*, size_t);
		template <typename type>  bool  BinarySortedTree(IBinaryTree<type>*); // TODO
		template <typename type>  void  MergeSort(type*, size_t);
		template <typename type>  void  QuickSort(type*, size_t, size_t);
		template <typename type>  void  SelectionSort(type*, size_t);

		template <typename type>  void  HeapSort(type*, size_t, bool(type, type));
		template <typename type>  void  InsertionSort(type*, size_t, bool(type, type));
		template <typename type>  bool  BinarySortedTree(IBinaryTree<type>*, bool(type, type)); // TODO
		template <typename type>  void  MergeSort(type*, size_t, bool(type, type));
		template <typename type>  void  QuickSort(type*, size_t, size_t, bool(type, type)); // TODO
		template <typename type>  void  SelectionSort(type*, size_t, bool(type, type)); // TODO
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

template <typename type>  bool  UltimaAPI::ISorters::more(type a, type b)
{
	return a > b;
}
template <typename type>  bool  UltimaAPI::ISorters::less(type a, type b)
{
	return a < b;
}
template <typename type>  bool  UltimaAPI::ISorters::more_equal(type a, type b)
{
	return a >= b;
}
template <typename type>  bool  UltimaAPI::ISorters::less_equal(type a, type b)
{
	return a <= b;
}

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
