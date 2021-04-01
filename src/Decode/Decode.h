#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <cstdio>
#include "DataBuffer.h"
#include "QRCode.h"
#include "Video.h"

using namespace cv;
using namespace std;

class Decode
{
	string videoFilename;
	string outputFilename;
	DataBuffer buffer;
	Video video;
public:
	Decode();
	Decode(string _outputFilename, string _filename);
	int video2Data();
};

