#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

class Video
{
	string filename;
	vector<Mat> img;
	unsigned int now;
	void init();
public:
	Video();
	Video(string _filename);
	bool loadfile(string _filename);
	bool isEnd();
	Mat nextImg();
	void resetPointer();
	int size();
	unsigned int pointer();
};

