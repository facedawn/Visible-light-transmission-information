#include "GenericGF.h"

GenericGF::GenericGF(int primitive, int size, int b)
{
	this->primitive = primitive;
	this->size = size;
	this->geneBase = b;

	expTab.resize(size);
	logTab.resize(size);

	int x = 1;
	int alpha = 2;

	for (int i = 0; i < size; i++)
	{
		expTab[i] = x;
		x *= 2;
		x = x >= size ? (x^primitive)&(size - 1) : x;
	}

	for (int i = 0; i < size - 1; i++) logTab[expTab[i]] = i;
}

std::string GenericGF::to_String()
{
	char ch[8];
	sprintf(ch, "%X", primitive);
	std::string str(ch);
	return "GF(0x" + str + "," + std::to_string(size) + ")";
}

