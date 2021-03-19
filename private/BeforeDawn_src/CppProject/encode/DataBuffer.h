#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class DataBuffer
{
    unsigned int now;
    unsigned int len;
    char* data;
    string filetype;

    void correcting();
    void zip();
public:
    DataBuffer();
    DataBuffer(string filename);

    bool reload(string filename);

    unsigned int size();
    unsigned int pointer();
    string getFiletype();
    char nextChar();
    bool isEnd();
    void resetPointer();
};
