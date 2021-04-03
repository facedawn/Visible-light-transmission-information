#pragma once

#include <cmath>
#include <bitset>

const int offlen = 12;
const int lenlen = 16 - offlen;
const int winlen = pow(2, offlen);
const int maxmch = pow(2, lenlen);

typedef std::bitset<offlen>  offset;
typedef std::bitset<lenlen> length;
typedef unsigned char byte;
typedef unsigned short word;

#define tonum(bit) (bit).to_ulong()
#define HASH 1009

class Record
{
public:
	offset off;
	length len;
	Record(offset o, length l)
	{
		off = o;
		len = l;
	}
	Record(word);
	word toWord();
};