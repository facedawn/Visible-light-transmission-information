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

	Mat frame;
	VideoCapture cap(filename.data());
	if (!cap.isOpened())
		return false;

	while (cap.read(frame))
	{
		Mat tmp;
		//极其重要！！！传输的是一个地址，并非Mat对象本身，如果只用frame，这个地址将会只有一个
		//每次新建frame的地址都是不一样的，而将frame赋值过去之后存储的地址也将不一样
		//在传递形参的时候会自动copyTo，所以并不会遇见这样子的问题
		frame.copyTo(tmp); 

		img.push_back(tmp);
	}
	cap.release();

	return true;
}

bool Video::isEnd()
{
	return (now == img.size());
}

Mat Video::nextImg()
{
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
unsigned int Video::pointer()
{
	return now;
}