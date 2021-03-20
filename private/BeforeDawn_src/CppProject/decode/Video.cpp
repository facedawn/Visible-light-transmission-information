#include "Video.h"

Video::Video()
{
	init();
}
Video::Video(string _filename)
{
	loadfile(_filename);
}
void Video::init()
{
	img.clear();
	now = 0;
	filename = "";
}

bool Video::loadfile(string _filename)
{
	init();
	filename = _filename;
    VideoCapture capture;
    Mat frame;
    if(!capture.open("f:/demo.mp4"))
		return false;

    while (capture.read(frame))
        img.push_back(frame);
    capture.release();

	return true;
}

bool Video::isEnd()
{
	return (now >= img.size());
}

Mat Video::nextImg()
{
	if (isEnd())
	{
		Mat tmp = img[0];
		return tmp;
	}
	return img[now++];
}
void Video::resetPointer()
{
	now = 0;
}
int Video::size()
{
	return img.size();
}