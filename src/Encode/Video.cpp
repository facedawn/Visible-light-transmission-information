#include "Video.h"

#define Rate 10.0 //Ö¡ÂÊ


Video::Video()
{
	clear();
	setFilename("");
}
Video::Video(string _filename)
{
	clear();
	setFilename(_filename);
}

void Video::setFilename(string _filename)
{
	if (_filename.size() != 0)
		filename = _filename;
	else
		filename = "demo.mp4";
}
bool Video::isEmpty()
{
	return (img.size() == 0);
}
void Video::append(Mat tmp)
{
	img.push_back(tmp);
}
void Video::clear()
{
	img.clear();
}

bool Video::saveVideo()
{
	if (isEmpty())
		return false;

	VideoWriter video(filename, CAP_OPENCV_MJPEG, Rate, Size(img[0].cols, img[0].rows));
	for (int i = 0; i < img.size(); i++)
		video << img[i];

	return true;
}

void Video::playVideo()
{
	Mat frame;
	VideoCapture cap(filename);
	if (cap.read(frame))
	{
		imshow("result", frame);
		printf("\n\nPress any key to play the video...\n");
		waitKey(0);
	}

	while (cap.read(frame))
	{
		imshow("result", frame);
		waitKey((int)(1000 / Rate));
	}
	cap.release();
	destroyAllWindows();
}