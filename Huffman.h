#ifndef __HUFFMAN_TREE_H__
#define __HUFFMAN_TREE_H__

#include <iostream>
using namespace std;

template <typename T>
struct HuffmanNode
{
	T _weight;
	HuffmanNode* _left;
	HuffmanNode* _right;
	HuffmanNode(T weight = 0) :_weight(weight), _left(NULL), _right(NULL)
	{}
	//建堆时要用
	//bool operator<(const HuffmanNode* right)
	//{
	//	return this->_weight < right->_weight;
	//}
};

template <typename T>
class HuffmanTree
{
	typedef HuffmanNode<T> Node;

public:
	HuffmanTree();
	HuffmanTree(const T* array, size_t size, int vailed = 0);
	Node* GetHuffmanNode();
	~HuffmanTree();
protected:
	static Node* _CreateHuffmanTree(const T* array, size_t size, int vailed = 0);
	static void _Clear(Node* root);
protected:
	Node* _root;
};

#endif 
