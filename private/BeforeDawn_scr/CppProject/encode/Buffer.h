#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class DataBuffer
{
    int now;
    int len;
    char* data;

public:
    DataBuffer();
    DataBuffer(string filename);

    bool reload(string filename);

    int size();
    char nextChar();
    bool isEnd();
    void resetPointer();
};
