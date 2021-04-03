#include "toprocess.h"

using namespace std;

byte ToProcess::pop()
{
	auto toRet = buffer[0];
	buffer.pop_front();
	return toRet;
}

void ToProcess::clear()
{
	buffer.clear();
}

void ToProcess::del(int i)
{
	if (i > buffer.size())
	{
		while (!buffer.empty())
		{
			buffer.pop_front();
		}
	}
	else
	{
		for (; i > 0; --i)
		{
			buffer.pop_front();
		}
	}
}