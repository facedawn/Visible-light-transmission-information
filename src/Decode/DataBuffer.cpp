#include "DataBuffer.h"

DataBuffer::DataBuffer()
{
	now = 0;
	total = 0;
	buffer.clear();
}
int DataBuffer::getNow()
{
	return now;
}
int DataBuffer::getTotal()
{
	return total;
}
void DataBuffer::setNow(int n)
{
	now = n;
}
void DataBuffer::setTotal(int t)
{
	total = t;
}
bool DataBuffer::isEnd()
{
	return !(now < total);
}


void DataBuffer::append(char tmp)
{
	buffer.push_back(tmp);
}
bool DataBuffer::savefile(string filename, string correctfile)
{
	uncorrect(correctfile);
	ofstream out(filename.data(), ios::out | ios::binary);
	if (!out)
		return false;
	char* tmp = new char[buffer.size() + 1];
	for (int i = 0; i < buffer.size(); i++)
		tmp[i] = (char)buffer[i];
	out.write(tmp, buffer.size());
	out.close();
	delete []tmp;
	return true;
}
bool DataBuffer::isEmpty()
{
	return (buffer.size() == 0);
}
void DataBuffer::clear()
{
	now = 0;
	total = 0;
	buffer.clear();
}
int DataBuffer::size()
{
	return buffer.size();
}

void DataBuffer::uncorrect(string correctfile)
{
	ofstream out(correctfile.data(), ios::out | ios::binary);
	const char tmp = -1; //先全部填写为1，表示全对。后期修改。
	for (int i = 0; i < buffer.size(); i++)
		out.write(&tmp, 1);
	out.close();
}