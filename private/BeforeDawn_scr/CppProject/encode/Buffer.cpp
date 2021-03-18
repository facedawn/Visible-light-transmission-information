#include "Buffer.h"


DataBuffer::DataBuffer()
{
    len = 0;
    data = NULL;
    resetPointer();
}
DataBuffer::DataBuffer(string filename)
{
    reload(filename);
}

bool DataBuffer::reload(string filename)
{
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
    data = new char(len + 1);

    in.seekg(0, ios::beg);
    in.read(data, len);

    return true;
}

int DataBuffer::size()
{
    return len;
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