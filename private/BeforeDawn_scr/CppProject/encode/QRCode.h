#pragma once
#include <opencv2/opencv.hpp>
#include "Buffer.h"

using namespace cv;

class QRCode
{
	DataBuffer* buffer;
	void QRCodeBasic();
	void writeData();
	void Xor1();
	void Xor2();
public:
	QRCode();
	QRCode(DataBuffer* _buffer);
	void setBuffer(DataBuffer* _buffer);

	Mat getQRCode();
};

