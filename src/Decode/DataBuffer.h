#pragma once
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class DataBuffer
{
	vector <char> buffer;
	int now;
	int total;
	void uncorrect(string correctfile);
public:
	DataBuffer();
	int getNow();
	int getTotal();
	void setNow(int n);
	void setTotal(int t);
	bool isEnd();

	void append(char tmp);
	bool savefile(string filename, string correctfile);
	bool isEmpty();
	void clear();
	int size();
};

