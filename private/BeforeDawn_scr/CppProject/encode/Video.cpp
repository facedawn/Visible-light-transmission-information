#include "Video.h"



Video::Video()
{
	setFilename("demo");
}
Video::Video(string _filename)
{
	setFilename(_filename);
}

void Video::setFilename(string _filename)
{
	filename = _filename;
}
bool Video::isEmpty()
{
	return (img.size() == 0);
}