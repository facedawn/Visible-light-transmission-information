#pragma once

#include <bitset>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <deque>
#include "window.h"
#include "toprocess.h"
#include "typedef.h"
#include "uncompress.h"

offset operator+=(offset&, int);
length operator+=(length&, int);
offset operator-=(offset&, int);
length operator-=(length&, int);

class LZ77
{
public:
	int encode(std::string, std::string);
	int encode(std::string, std::vector<byte>*);
	int decode(std::string, std::string);
	int decode(std::vector<byte>*, std::string);
	~LZ77();
private:
	int encode(std::string);
	int decode(unsigned int);
	std::vector<byte> cmpBuffer;
	std::vector<std::bitset<1> > corrTable;
	Window window;
	ToProcess orgdata;
	UnCompress ucpBuffer;

	void bufferToFile(std::string);
	void writeToBuffer(word);
	void writeToBuffer(byte b);
	word match();
};