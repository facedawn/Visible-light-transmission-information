#pragma once

#include "typedef.h"
#include <vector>

class UnCompress
{
public:
	void append(byte b)
	{
		buffer.push_back(b);
	}
	void append(Record);
	byte operator[](int i)
	{
		return buffer[i];
	}
	void clear()
	{
		buffer.clear();
	}
	int size()
	{
		return buffer.size();
	}
private:
	std::vector<byte> buffer;
};
