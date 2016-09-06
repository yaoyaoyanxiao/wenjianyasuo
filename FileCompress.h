#ifndef __HUFFMAN_CODE_H__
#define __HUFFMAN_CODE_H__

#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include "Huffman.h"
#include "Huffman.hpp"


typedef unsigned long long TypeLong;

struct CharInfo
{
	unsigned char _ch; //字符
	TypeLong _count;//出现次数
	string _code;//Huffman编码

	CharInfo(TypeLong count = 0) :_ch(0), _count(count)
	{}
	bool operator!=(const CharInfo& info)const
	{
		return this->_count != info._count;
	}
	bool operator<(const CharInfo& info)const
	{
		return this->_count < info._count;
	}
	CharInfo operator+(const CharInfo& info)const
	{
		return CharInfo(this->_count + info._count);
	}
};

class FileCompress
{
public:
	FileCompress();
	void CompressHuffCode(const char* filename);
	void UnCompressHuffCode(const char* filename);
	void PrintCode()const;

protected:
	static void GenerateHuffmanCode(HuffmanNode<CharInfo>* root, FileCompress& file, string& code);

protected:
	CharInfo _info[256];

};

#endif 
