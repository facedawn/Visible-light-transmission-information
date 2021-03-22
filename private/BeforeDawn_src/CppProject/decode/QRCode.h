#pragma once
#include <opencv2/opencv.hpp>
#include "DataBuffer.h"
#include "QRList.h"
#include "AnchorPoint.h"
#include "BlockCoordinates.h"
#include "Points.h"

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

