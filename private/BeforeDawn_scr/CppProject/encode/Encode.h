#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>

#include "Buffer.h"
#include "Video.h"

using namespace std;
using namespace cv;

class Encode
{
	DataBuffer buffer;
	vector<Mat> img;
	Video video;
public:
	Encode();
	Encode(string filename, string videoFilename);
	Encode(DataBuffer _buffer, string videoFilename);

	bool isEmpty();
	bool data2Video();
};

