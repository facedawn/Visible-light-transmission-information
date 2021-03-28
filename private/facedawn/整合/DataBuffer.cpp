#include "DataBuffer.h"

DataBuffer::DataBuffer()
{
	buffer.clear();
}
void DataBuffer::append(char tmp)
{
	buffer.push_back(tmp);
}
bool DataBuffer::savefile(string filename)
{
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
	buffer.clear();
}