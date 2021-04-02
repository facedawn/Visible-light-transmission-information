#pragma once
#include <fstream>
#include <vector>

using namespace std;

class DataBuffer
{
	vector <char> buffer;
	int now;
	int total;
public:
	DataBuffer();
	int getNow();
	int getTotal();
	void setNow(int n);
	void setTotal(int t);
	bool isEnd();

	void append(char tmp);
	bool savefile(string filename);
	bool isEmpty();
	void clear();
	int size();
};

