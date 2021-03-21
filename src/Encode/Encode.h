#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>

#include "DataBuffer.h"
#include "Video.h"
#include "QRCode.h"

using namespace std;
using namespace cv;

class Encode
{
	DataBuffer buffer;
	Video video;
public:
	Encode();
	Encode(string filename, string videoFilename);
	Encode(DataBuffer _buffer, string videoFilename);

	bool isEmpty();
	bool data2Video();
};

