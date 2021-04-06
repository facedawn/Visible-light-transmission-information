#include <iostream>
#include "LZ77.h"

//#define MAIN 0

using namespace std;

inline int getLen(short s)
{
    s &= (word)pow(2, lenlen) - 1;
    return (int)s;
}

offset operator+=(offset& o, int i)
{
    o = o.to_ulong() + i;
    return o;
}

length operator+=(length& l, int i)
{
    l = l.to_ulong() + i;
    return l;
}

offset operator-=(offset& o, int i)
{
    o = o.to_ulong() - i;
    return o;
}

length operator-=(length& l, int i)
{
    l = l.to_ulong() - i;
    return l;
}

word Record::toWord()
{
    word w = 0;
    bitset<16> tmp;
    for (int i = 0; i < 16; i++)
    {
        if (i < lenlen)tmp[i] = len[i];
        else tmp[i] = off[i - lenlen];
    }
    w = tmp.to_ulong();
    return w;
}

Record::Record(word w)
{
    len = getLen(w);
    w -= tonum(len);
    w >>= lenlen;
    off = w;
}

void LZ77::writeToBuffer(word record)
{
    cmpBuffer.push_back((byte)((record & 0xff00) >> 8));
    cmpBuffer.push_back((byte)record);
    corrTable.push_back(1);
}

void LZ77::writeToBuffer(byte b)
{
    cmpBuffer.push_back(b);
    corrTable.push_back(0);
}

void LZ77::bufferToFile(string dst)
{

    ofstream file(dst, ofstream::binary | ofstream::out);
    unsigned int tablelen = corrTable.size();
    file.write((const char*)&tablelen, sizeof(unsigned int));
    for (int b = 0; b < tablelen / 8 ; ++b)
    {
        byte tmp = 0;
        for (int bit = 0; ; ++bit)
        {
            if (corrTable[b * 8 + bit] == 1)tmp += 1;
            if (bit != 7)tmp <<= 1;
            else break;
        }
        file.write((const char*)&tmp, sizeof(byte));
    }

    int remlen = tablelen - (tablelen / 8 * 8);
    byte tmp = 0;
    for (int bit = 0; bit < remlen; bit++)
    {
        byte tmp;
        if (corrTable[(tablelen / 8 * 8) + bit] == 1)tmp = 1;
        else tmp = 0;
        file.write((const char*)&tmp, sizeof(byte));
    }

    for (byte b : cmpBuffer)
    {
        file.write((const char*)&b, 1);
    }
    file.close();
    corrTable.clear();
    cmpBuffer.clear();
}

word LZ77::match()
{
    if (window.size() < 3)
    {
        return 0;
    }
    unsigned int hash = 1;
    for (int i = 0; i < 3; ++i)
    {
        if (orgdata[i] != 0)
            hash *= orgdata[i];
    }
    auto r = window.search(hash % HASH);
    if (r->empty())
    {
        return 0;
    }
    
    int maxLen = 1;
    int maxOff = 0;
    for (auto i : *r)
    {
        i -= window.headIndex();
        
        if (i + maxLen - 1 >= window.size())
            break;
        if (window[i + maxLen - 1] != orgdata[maxLen - 1])
            continue;
        
        int len = 0;
        for (int j = 0, k = i; (len < maxmch - 1) && (k < window.size()) && j < orgdata.size(); ++k, ++j)
        {
            if (window[k] == orgdata[j])
                ++len;
            else
                break;
        }
        if (len >= maxLen)
        {
            maxLen = len;
            maxOff = window.size() - i;
        }
    }

    if (maxLen >= 3)
    {
        return Record(maxOff, maxLen).toWord();
    }
    else
    {
        return 0;
    }
}

int LZ77::encode(string src)
{
    ifstream in(src, ifstream::binary | ifstream::in);
    if (!in)
    {
        return -1;
    }
    while (in.peek() != EOF)
    {
        byte tmp;
        in.read((char*)&tmp, 1);
        orgdata.push(tmp);
    }
    in.close();

    int byteCnt = 0;
    int size = orgdata.size();
    int bcnt = 0, wcnt = 0;
    for (byteCnt = 0; byteCnt < size - 2; )
    {
        byte b = orgdata[0];
        word s = match();
        if (s == 0)
        {
            ++bcnt;
            writeToBuffer(b);
            window.append(orgdata.pop());
            ++byteCnt;
        }
        else
        {
            ++wcnt;
            writeToBuffer(s);
            auto len =  getLen(s);
            byteCnt += len;
            for (int i = 0; i < len; ++i)
            {
                window.append(orgdata.pop());
            }
        }
    }
    while (!orgdata.empty())
    {
        writeToBuffer(orgdata.pop());
        ++byteCnt;
        ++bcnt;
    }
    int total = 0;
    total += (bcnt + wcnt) / 8;
    total += 4;
    total += (bcnt + wcnt) - ((bcnt + wcnt) / 8) * 8;
    total += bcnt + wcnt * 2;
    return total;
}

int LZ77::encode(string src, string dst)
{
    int size = encode(src);
    if(size != -1)
        bufferToFile(dst);
    return size;
}

int LZ77::encode(string src, vector<byte> *buffer)
{
    int size = encode(src);
    if (size == -1)
        return size;
    buffer->clear();
    unsigned int tablelen = corrTable.size();
    for (int i = 0; i < sizeof(unsigned int); ++i)
    {
        int shift = 8 * (sizeof(unsigned int) - i - 1);
        buffer->push_back((byte)(tablelen >> shift));
    }
    for (int b = 0; b < tablelen / 8; ++b)
    {
        byte tmp = 0;
        for (int bit = 0; ; ++bit)
        {
            if (corrTable[b * 8 + bit] == 1)tmp += 1;
            if (bit != 7)tmp <<= 1;
            else break;
        }
        buffer->push_back(tmp);
    }

    int remlen = tablelen - (tablelen / 8 * 8);
    byte tmp = 0;
    for (int bit = 0; bit < remlen; bit++)
    {
        byte tmp;
        if (corrTable[(tablelen / 8 * 8) + bit] == 1)tmp = 1;
        else tmp = 0;
        buffer->push_back(tmp);
    }

    for (byte b : cmpBuffer)
    {
        buffer->push_back(b);
    }
    corrTable.clear();
    cmpBuffer.clear();

    return size + 4;
}

int LZ77::decode(unsigned int tableLen)
{
    if (!corrTable.empty())
        corrTable.clear();
    for (int i = 0; i < (tableLen / 8); ++i)
    {
        byte b = 128;
        byte bits = orgdata.pop();
        for (int j = 0; j < 8; ++j)
        {
            if (bits & (b >> j))
                corrTable.push_back(1);
            else
                corrTable.push_back(0);
        }
    }
    for (int i = 0; i < tableLen - (tableLen / 8) * 8; ++i)
    {
        if(orgdata.pop())
            corrTable.push_back(1);
        else
            corrTable.push_back(0);
    }

    for (auto flag : corrTable)
    {
        if (tonum(flag))
        {
            word w = 0;
            w += orgdata.pop();
            w <<= 8;
            w += orgdata.pop();
            ucpBuffer.append(Record(w));
        }
        else
        {
            ucpBuffer.append(orgdata.pop());
        }
    }
    return ucpBuffer.size();
}

int LZ77::decode(string src, string dst)
{
    ifstream in(src, ifstream::binary | ifstream::in);
    if (!in)
        return -1;
    unsigned int tableLen;
    in.read((char*)&tableLen, sizeof(unsigned int));
    while (in.peek() != EOF)
    {
        byte tmp;
        in.read((char*)&tmp, 1);
        orgdata.push(tmp);
    }
    in.close();

    int size = decode(tableLen);
    ofstream out(dst, ifstream::binary | ifstream::out);
    for (int i = 0; i < ucpBuffer.size(); ++i)
    {
        byte tmp = ucpBuffer[i];
        out.write((const char*)&tmp, 1);
    }
    out.close();
    return size;
}

int LZ77::decode(vector<byte>* buffer, string dst)
{
    unsigned int tableLen = 0;
    for (int i = 0; i < sizeof(unsigned int); ++i)
    {
        tableLen += (*buffer)[i];
        tableLen << 8;
        if (i == sizeof(unsigned int) - 1)break;
    }
    if (orgdata.size() != 0)orgdata.clear();
    for (int i = sizeof(unsigned int); i < (*buffer).size(); ++i)
        orgdata.push((*buffer)[i]);

    int size = decode(tableLen);
    if (size == -1)
        return size;
    ofstream out(dst, ifstream::binary | ifstream::out);
    for (int i = 0; i < ucpBuffer.size(); ++i)
    {
        byte tmp = ucpBuffer[i];
        out.write((const char*)&tmp, 1);
    }
    out.close();
    return size;
}

LZ77::~LZ77()
{
    cmpBuffer.clear();
    corrTable.clear();
    ucpBuffer.clear();

}
#ifdef MAIN

#include <ctime>
int main()
{
    vector <byte> v;
    LZ77 e = LZ77();
    auto begin = clock();
    auto size = e.encode("F:\\bigtry.exe", "F:\\sample\\btrydst");
    cout<<"压缩文件大小："<< size << " bytes" <<endl;
    auto end = clock();
    cout <<"压缩耗时："<< (int)(end - begin) << "ms" << endl;
    if (end - begin != 0)
        cout << "平均压缩速率："<< (size / (int)(end - begin)) / 1024.0 * 1000 << "KB/s" <<endl;

    begin = clock();
    cout << "解压文件大小：" <<e.decode("F:\\sample\\btrydst", "F:\\sample\\ucpbtry.exe") << " bytes" <<endl;
    end = clock();
    cout << "解压耗时：" << (int)(end - begin) << "ms" << endl;
    
    begin = clock();
    cout << e.encode("F:\\bigtry.exe", &v) << endl;
    end = clock();
    cout << end - begin << endl;
    if (end - begin != 0)
        cout << "平均压缩速率：" << (size / (int)(end - begin)) / 1024.0 * 1000 << "KB/s" << endl;

    begin = clock();
    cout << e.decode(&v, "F:\\bigtrycopy.exe") << endl;
    end = clock();
    cout << end - begin << endl;
}
#endif //MAIN