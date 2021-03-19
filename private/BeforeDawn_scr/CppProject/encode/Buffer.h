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
    string type;

public:
    DataBuffer();
    DataBuffer(string filename);

    bool reload(string filename);
    void correcting();
    void zip();

    int size();
    int pointer();
    string filetype();
    char nextChar();
    bool isEnd();
    void resetPointer();
};
