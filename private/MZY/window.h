#pragma once

#include "typedef.h"
#include <deque>
#include <vector>
#include <list>

class Window
{
public:
	void append(byte);
	int headIndex()
	{
		return winHead;
	}
	byte operator[](int i)
	{
		return w[i];
	}
	std::list<int>* search(unsigned int hash)
	{
		return hashArray[hash];
	}
	int size()
	{
		return w.size();
	}
	void clear();
	~Window()
	{
		for (int i = 0; i < HASH; ++i)
			delete hashArray[i];
		delete hashArray;
	}

	Window()
	{
		for (int i = 0; i < HASH; ++i)
			hashArray[i] = new std::list<int>();
	}

private:
	int winHead = 0;
	int total = 0;
	void addHash();
	std::list<int>* *hashArray = new std::list<int>* [HASH];
	std::deque<byte> w;
};