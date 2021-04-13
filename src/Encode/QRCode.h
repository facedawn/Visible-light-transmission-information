#pragma once
#include <opencv2/opencv.hpp>
#include <random>
#include <vector>
#include <cstdio>
#include "DataBuffer.h"

using namespace cv;
using namespace std;

class QRCode
{
	DataBuffer* buffer;
	void init();
	void QRCodeBasic();
	void writeData(int length);
	void Xor();
	void fixPoint(int& x, int& y);
public:
	QRCode();
	QRCode(DataBuffer* _buffer);
	void setBuffer(DataBuffer* _buffer);

	Mat getQRCode(int length);
	bool isEnd();
};

