#ifndef __HEAP_HPP__
#define __HEAP_HPP__

#include "Heap.h"

template <typename T, template<class> class Compare>
Heap<T, Compare>::Heap()
{}

template <typename T, template<class> class Compare>
Heap<T, Compare>::~Heap()
{}

template <typename T, template<class> class Compare>
Heap<T, Compare>::Heap(const T* array, size_t size)
{
	this->_array.resize(size);
	for (size_t i = 0; i < size; ++i)
	{
		this->_array[i] = array[i];
	}
	for (int i = (size - 2) / 2; i >= 0; --i)
	{
		this->_AdjustDown(i);
	}
}

template <typename T, template<class> class Compare>
Heap<T, Compare>::Heap(const Heap<T, Compare>& heap)
{
	size_t size = heap.size();
	this->_array.resize(size);

	for (size_t i = 0; i < size; ++i)
	{
		this->_array[i] = heap._array[i];
	}
}

template <typename T, template<class> class Compare>
Heap<T, Compare>& Heap<T, Compare>::operator=(const Heap<T, Compare>& heap)
{
	if (this != &heap)
	{
		this->_array = heap._array;
	}
	return *this;
}

template <typename T, template<class> class Compare>
void Heap<T, Compare>::Push(const T& x)
{
	size_t size = this->_array.size();
	this->_array.push_back(x);
	this->_AdjustUp(size);
}

//Compare
template <typename T, template<class> class Compare>
void Heap<T, Compare>::_AdjustUp(int pos)
{
	assert(pos<this->_array.size());
	Compare<T> com;
	int parent = (pos - 1) / 2;
	int child = pos;

	while (parent >= 0 && com(this->_array[child], this->_array[parent]))
	{
		swap(this->_array[child], this->_array[parent]);
		child = parent;
		parent = (child - 1) / 2;
	}
}

template <typename T, template<class> class Compare>
void Heap<T, Compare>::Pop()
{
	assert(!this->_array.empty());
	size_t size = this->_array.size();
	swap(this->_array[0], this->_array[size - 1]);
	this->_array.pop_back();
	this->_AdjustDown(0);
}

//Compare
template <typename T, template<class> class Compare>
void Heap<T, Compare>::_AdjustDown(int parent)
{
	size_t child = parent * 2 + 1;
	size_t size = this->_array.size();
	Compare<T> com;
	while (child < size)
	{
		if (child + 1 < size && com(this->_array[child + 1], this->_array[child]))
		{
			++child;
		}
		if (com(this->_array[child], this->_array[parent]))
		{
			swap(this->_array[parent], this->_array[child]);
			parent = child;
			child = parent * 2 + 1;
		}
		else
		{
			break;
		}
	}
}

template <typename T, template<class> class Compare>
T Heap<T, Compare>::Top()const
{
	assert(!this->_array.empty());
	return this->_array[0];
}

template <typename T, template<class> class Compare>
bool Heap<T, Compare>::Empty()const
{
	return this->_array.empty();
}

template <typename T, template<class> class Compare>
size_t Heap<T, Compare>::Size()const
{
	return this->_array.size();
}

template <typename T, template<class> class Compare>
std::ostream& operator<<(std::ostream& out, const Heap<T, Compare>& heap)
{
	size_t size = heap._array.size();
	for (size_t i = 0; i < size; ++i)
	{
		out << heap._array[i] << " ";
	}
	out << endl;
	return out;
}

template <typename T, template<class> class Compare>
void HeapSort(T* array, size_t size, const Compare<T>& com)
{
	CrtHeap(array, size, com);//建初堆
	for (int i = size - 1; i > 0; --i)
	{
		swap(array[0], array[i]); //交换头和尾
		AdjustDown(array, i, 0, com); //使得0...i-1也为堆
	}
}

//建初堆
template <typename T, template<class> class Compare>
void CrtHeap(T* array, size_t size, const Compare<T>& com)
{
	int parent = (size - 2) / 2;
	for (int i = parent; i >= 0; --i)
	{
		AdjustDown(array, size, i, com);
	}
}

//下调 ，Compare
template <typename T, template<class> class Compare>
void AdjustDown(T* array, size_t size, int parent, const Compare<T>& com)
{
	int child = parent * 2 + 1;
	while (child < (int)size)
	{
		if (child + 1 < size && com(array[child + 1], array[child]))
		{
			++child;
		}
		if (com(array[child], array[parent]))
		{
			swap(array[parent], array[child]);
			parent = child;
			child = parent * 2 + 1;
		}
		else
		{
			break;
		}
	}
}

template <typename T, template<class> class Compare>
void GetTopK(T* array, const vector<T>& money, const size_t& k, const size_t& n, const Compare<T>& com)
{
	assert(array);
	assert(k < n);
	for (size_t i = 0; i < k; ++i)
	{
		array[i] = money[i];
	}
	//建堆
	for (int i = (k - 2) / 2; i >= 0; --i)
	{
		AdjustDown(array, k, i, com);
	}
	for (int i = k; i < n; ++i)
	{
		if (com(array[0], money[i]))
		{
			array[0] = money[i];
			AdjustDown(array, k, 0, com);
		}
	}
}
#endif
