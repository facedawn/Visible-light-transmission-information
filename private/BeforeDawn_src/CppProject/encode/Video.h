#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdio>

using namespace std;
using namespace cv;

class Video
{
	string filename;
	vector<Mat> img;
public:
	Video();
	Video(string _filename);

	void setFilename(string _filename);
	bool isEmpty();
	void append(Mat tmp);
	void clear();

	bool saveVideo();
	void playVideo();
};

