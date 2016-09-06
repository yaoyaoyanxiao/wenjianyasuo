#define _CRT_SECURE_NO_WARNINGS
#ifndef __HUFFMAN_CODE_CPP__
#define __HUFFMAN_CODE_CPP__

#include<iostream>
using namespace std;
#include <assert.h>
#include "FileCompress.h"

FileCompress::FileCompress()
{
	size_t size = sizeof(this->_info) / sizeof(this->_info[0]);
	for (size_t i = 0; i < size; ++i)
	{
		this->_info[i]._ch = i;
		this->_info[i]._count = 0;
	}
}

void FileCompress::CompressHuffCode(const char* filename)
{
	assert(filename);
	FILE* fOut = fopen(filename, "rb");
	assert(fOut);
	//统计字符出现的次数
	char ch = fgetc(fOut);
	while (!feof(fOut))
	{
		++this->_info[(unsigned char)ch]._count;
		ch = fgetc(fOut);
	}

	//建立哈夫曼树
	HuffmanTree<CharInfo> huffTree(this->_info, sizeof(this->_info) / sizeof(this->_info[0]), 0);

	//生成哈夫曼编码
	string code;
	HuffmanNode<CharInfo>* root = huffTree.GetHuffmanNode();
	GenerateHuffmanCode(root, *this, code);

	//生成压缩文件名及配置文件名
	string fileInName = (string)filename; //压缩文件名
	string fileConfig = fileInName;//配置文件名
	size_t last_ = fileInName.find_last_of('.');
	if (last_ < fileInName.size())
	{
		fileInName.erase(last_);
		fileConfig.erase(last_);
	}
	fileInName += ".huff";
	fileConfig += ".config";

	//fileInName = "E:\\姚瑶文件压缩文件\\yaoyaoh";
	//fileConfig = "E:\\姚瑶文件压缩文件\\yaoyao.config";

	string tmp;

	//生成压缩配置文件
	FILE* fConfig = fopen(fileConfig.c_str(), "wb");
	char buff[20] = { 0 };
	for (size_t i = 0; i < sizeof(this->_info) / sizeof(this->_info[0]); ++i)
	{
		if (this->_info[i]._count != 0)
		{
			tmp += this->_info[i]._ch;
			tmp += ':';
			tmp += (string)_itoa((this->_info[i]._count), buff, 10);
			tmp += '\n';
			fputs(tmp.c_str(), fConfig);
			tmp.clear();
		}
	}

	//对文件进行压缩
	FILE* fIn = fopen(fileInName.c_str(), "wb");
	assert(fIn);
	fseek(fOut, 0, SEEK_SET);
	int pos = 0;
	unsigned char putch = 0;
	ch = fgetc(fOut);
	while (!feof(fOut))
	{
		tmp = this->_info[(unsigned char)ch]._code;
		for (size_t i = 0; i < tmp.size(); ++i)
		{
			putch <<= 1;
			putch |= (tmp[i] - '0');
			if (++pos == 8)
			{
				fputc(putch, fIn);
				pos = 0;
				putch = 0;
			}
		}
		ch = fgetc(fOut);
	}
	if (pos > 0)
	{
		putch <<= (8 - pos);
		fputc(putch, fIn);
	}

	fclose(fOut);
	fclose(fIn);
	fclose(fConfig);
}

void FileCompress::GenerateHuffmanCode(HuffmanNode<CharInfo>* root, FileCompress& file, string& code)
{
	if (root == NULL)
	{
		return;
	}
	if (root->_left == NULL && root->_right == NULL)
	{
		file._info[root->_weight._ch]._code = code;
		return;
	}
	code.push_back('0');
	GenerateHuffmanCode(root->_left, file, code);
	code.pop_back();
	code.push_back('1');
	GenerateHuffmanCode(root->_right, file, code);
	code.pop_back();
}

void FileCompress::UnCompressHuffCode(const char* filename)
{
	assert(filename);
	FILE* fOut = fopen(filename, "rb");
	assert(fOut);
	//读取文件，
	string fileConfig = (string)filename;
	string fileInName = fileConfig;
	size_t last_ = fileInName.find_last_of('.');
	if (last_ < fileInName.size())
	{
		fileConfig.erase(last_);
		fileInName.erase(last_);
	}
	fileConfig += ".config";
	fileInName += "_Com.txt";

	//fileInName = "E:\\姚瑶文件压缩文件\\yaoyao.txt";
	//fileConfig = "E:\\姚瑶文件压缩文件\\yaoyao.config";

	FILE* fIn = fopen(fileInName.c_str(), "wb");
	assert(fIn);
	FILE* fConfig = fopen(fileConfig.c_str(), "rb");
	assert(fConfig);

	//修改_count,注意\n，有可能代表字符，有可能是行结束标志
	char buff[20] = { 0 };
	unsigned char ch = fgetc(fConfig);
	while (!feof(fConfig))
	{
		fgetc(fConfig);//读取冒号
		fgets(buff, 20, fConfig);
		this->_info[ch]._count = (TypeLong)atoi((buff));
		ch = fgetc(fConfig);
	}

	//重建哈夫曼树
	HuffmanTree<CharInfo> tree(this->_info, sizeof(this->_info) / sizeof(this->_info[0]), 0);
	HuffmanNode<CharInfo>* root = tree.GetHuffmanNode();
	HuffmanNode<CharInfo>* cur = root;
	TypeLong countSum = root->_weight._count; //记录字符的总个数控制结束
	ch = fgetc(fOut);
	int pos = 7;
	while (countSum > 0)
	{
		while (pos >= 0)
		{
			if ((ch & (1 << pos)) == 0) //向左走
			{
				cur = cur->_left;
			}
			else
			{
				cur = cur->_right;
			}
			if (cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch, fIn);
				//cout << cur->_weight._ch;
				
				if (--countSum == 0)//将没有写的字符的次数减1
					break;
				cur = root;
			}
			--pos;
		}
		pos = 7;
		ch = fgetc(fOut);
	}
	fclose(fIn);
	fclose(fOut);
	fclose(fConfig);
}

void FileCompress::PrintCode()const
{
	for (int i = 0; i < 256; ++i)
	{
		if (this->_info[i]._count != 0)
		{
			cout << this->_info[i]._ch << ":>" << this->_info[i]._code << endl;
		}
	}
}
#endif
