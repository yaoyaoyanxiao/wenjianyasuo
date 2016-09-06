#ifndef __HEAP_H__
#define __HEAP_H__

#include <iostream>
#include <assert.h>
#include <vector>
#include "FileCompress.h"
#include "Huffman.hpp"
#include "Huffman.h"

template <typename T>
struct Less
{
	bool operator()(const T& left, const T& right)const
	{
		return left < right;
	}
};

template<typename K> //模板的偏特化
struct Less<HuffmanNode<K>*>
{
	bool operator()(const HuffmanNode<K>* left, const HuffmanNode<K>* right)const
	{
		return left->_weight < right->_weight;
	}
};

template <typename T>
struct Greater
{
	bool operator()(const T& left, const T& right)const
	{
		return left > right;
	}
};

template <typename T, template<class> class Compare = Greater>
class Heap
{
	friend std::ostream& operator<<<T, Compare>(std::ostream& out, const Heap<T, Compare>& heap);
public:
	Heap();
	Heap(const T* array, size_t size);
	Heap(const Heap<T, Compare>& heap);
	Heap<T, Compare>& operator=(const Heap<T, Compare>& heap);
	~Heap();

	void Push(const T& x);
	void Pop();
	T Top()const;
	bool Empty()const;
	size_t Size()const;

protected:
	void _AdjustDown(int parent);
	void _AdjustUp(int pos);

protected:
	std::vector<T> _array;
};

//堆排序
template <typename T, template<class> class Compare = Greater>
void HeapSort(T* array, size_t size, const Compare<T>& com = Compare<T>());
//建初堆
template <typename T, template<class> class Compare = Greater>
void CrtHeap(T* array, size_t size, const Compare<T>& com = Compare<T>());
//下调
template <typename T, template<class> class Compare = Greater>
void AdjustDown(T* array, size_t size, int parent = 0, const Compare<T>& com = Compare<T>());

template <typename T, template<class> class Compare = Greater>
std::ostream& operator<<(std::ostream& out, const Heap<T, Compare>& heap);

template <typename T, template<class> class Compare = Greater>
void GetTopK(T* array, const vector<T>& money, const size_t& k, const size_t& n, const Compare<T>& com = Compare<T>());

#endif 
