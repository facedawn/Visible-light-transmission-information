#pragma once
#include <fstream>
#include <vector>

using namespace std;

class DataBuffer
{
	vector <char> buffer;
public:
	DataBuffer();
	void append(char tmp);
	bool savefile(string filename);
	bool isEmpty();
	void clear();
};

