#pragma once

#include "typedef.h"
#include <deque>

class ToProcess
{
public:
	void clear();
	byte pop();
	void del(int);
	void push(byte b)
	{
		buffer.push_back(b);
	}
	byte operator[](int i)
	{
		return buffer[i];
	}
	int size()
	{
		return buffer.size();
	}
	bool empty()
	{
		return buffer.empty();
	}
private:
	std::deque<byte> buffer;
};