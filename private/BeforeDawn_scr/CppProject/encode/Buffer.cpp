#include "Buffer.h"

#include <stdio.h>
#include <iostream>

using namespace std;


DataBuffer::DataBuffer()
{
    len = 0;
    data = NULL;
    resetPointer();
}
DataBuffer::DataBuffer(string filename)
{
    data = NULL;
    reload(filename);
}

bool DataBuffer::reload(string filename)
{
    if (data != NULL)
        delete data;
    ifstream in(filename.data(), ios::in | ios::binary);
    if (!in)
    {
        len = 0;
        data = NULL;
        return false;
    }
    resetPointer();

    long long start = in.tellg();
    in.seekg(0, ios::end);
    long long end = in.tellg();
    len = end - start;
    data = new char[len + 10];

    in.clear();
    in.seekg(0, ios::beg);
    in.read(data, len);
    in.close();

    zip();
    correcting();

    int index = filename.rfind(".");
    printf("%d\t%d\n", index, filename.size());
    type = filename.substr(index, filename.size() - index);
    cout << filename << endl << type << endl;

    return true;
}
void DataBuffer::correcting()
{
    //在这里写纠错码的部分
    //这部分的优先级较低，可能会在最后完成
}
void DataBuffer::zip()
{
    //在这里写压缩的部分
    //这部分的优先级很低，可能并不会写
}

int DataBuffer::size()
{
    return len;
}
int DataBuffer::pointer()
{
    return now;
}
string DataBuffer::filetype()
{
    return type;
}
char DataBuffer::nextChar()
{
    if (now < len)
        return data[now++];
    else
        return (char)0;
}
bool DataBuffer::isEnd()
{
    return now == len;
}
void DataBuffer::resetPointer()
{
    now = 0;
}