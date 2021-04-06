#include "window.h"

using namespace std;

void Window::append(byte b)
{
	++total;
	w.push_back(b);
	if (w.size() >= 3)
		addHash();
	if (w.size() >= winlen)
	{
		unsigned int hash = 1;
		for (int i = 0; i < 3; ++i)
		{
			if (w[i] != 0)
				hash *= w[i];
		}
		w.pop_front();
		hashArray[hash % HASH]->pop_front();
		++winHead;
	}
}

void Window::clear()
{
	total = 0;
	winHead = 0;
	w.clear();
	for (int i = 0; i < HASH; ++i)
		hashArray[i]->clear();
}

void Window::addHash()
{
	unsigned int hash = 1;
	int first = w.size() - 3;
	for (int i = 0; i < 3; ++i)
	{
		if (w[first + i] != 0)
			hash *= w[first + i];
	}
	hash %= HASH;
	hashArray[hash]->push_back(total - 3);
}