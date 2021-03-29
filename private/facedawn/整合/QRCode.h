#pragma once
#include <opencv2/opencv.hpp>
#include <cstdio>
#include "DataBuffer.h"
#include "QRList.h"
#include "QRMatrix.h"
#include "QRLocation.h"
#include "GlobalMacroDefine.h"

using namespace cv;

class QRCode
{
	DataBuffer* buffer;

	QRList locationQR(Mat img);

	void fixPoint(int& x, int& y);
	bool versionCheck(QRMatrix &matrix);
	bool sizeCheck(QRMatrix& matrix);
	bool EffectivenessCheck(QRMatrix& matrix);
	void read(QRMatrix& matrix);
public:
	QRCode();
	QRCode(DataBuffer* dataBuffer);
	void setBuffer(DataBuffer* dataBuffer);
	bool decode(Mat img);
};

