#pragma once

#include "ISorter.h"

#if defined(BINARYSORTEDTREE) && BINARYSORTEDTREE

template <typename type>
class UltimaAPI::ISorters::IBinaryTree
{
	IBinaryTree* left, * right;
#pragma pack(push, 4)
	type data;
#pragma pack(pop)
};

template <typename type>
bool  UltimaAPI::ISorters::BinarySortedTree(IBinaryTree<type>* node)
{
	static struct node* prev = nullptr;

	if (node)
	{
		if (!isBST(node->left))
			return false;

		if (prev != NULL && node->data <= prev->data)
			return false;

		prev = node;

		return BinarySortedTree(node->right);
	}
	return true;
}

template <typename type>
bool  UltimaAPI::ISorters::BinarySortedTree(IBinaryTree<type>* node, bool function(type, type))
{
	throw(L"TODO");

	static struct node* prev = nullptr;

	if (node)
	{
		if (!isBST(node->left))
			return false;

		if (prev != nullptr && function(node->data, prev->data))
			return false;

		prev = node;

		return BinarySortedTree(node->right, function);
	}
	return true;
}

#else
#if defined(EXCEPTIONS) && EXCEPTIONS

template <typename type>
bool  UltimaAPI::ISorters::BinarySortedTree(IBinaryTree<type>*)
{
	throw(ExceptionISorter(L"BinarySortedTree", 0));
}

template <typename type>
bool  UltimaAPI::ISorters::BinarySortedTree(IBinaryTree<type>*, bool(type, type))
{
	throw(ExceptionISorter(L"BinarySortedTree", 1));
}

#else

template <typename type>
bool  UltimaAPI::ISorters::BinarySortedTree(IBinaryTree<type>*)
{
	throw(L"BinarySortedTreeException");
}

template <typename type>
bool  UltimaAPI::ISorters::BinarySortedTree(IBinaryTree<type>*, bool(type, type))
{
	throw(L"BinarySortedTreeException");
}

#endif
#endif