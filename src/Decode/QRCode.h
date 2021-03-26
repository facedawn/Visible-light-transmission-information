#pragma once
#include <opencv2/opencv.hpp>
#include "DataBuffer.h"
#include "QRList.h"
#include "QRMatrix.h"

using namespace cv;

class QRCode
{

	DataBuffer* buffer;

	QRList locationQR(Mat img);
public:
	QRCode();
	QRCode(DataBuffer* dataBuffer);
	void setBuffer(DataBuffer* dataBuffer);
	bool decode(Mat img);
};

