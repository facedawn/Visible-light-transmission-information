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
	unsigned int byte;

	QRList locationQR(Mat& img);

	void fixPoint(int& x, int& y);
	void QRCodeBasic();
	bool versionCheck(QRMatrix& matrix);
	bool sizeCheck(QRMatrix& matrix);
	void Xor(QRMatrix& matrix);

	bool EffectivenessCheck(QRMatrix& matrix, int& x, int& y);
	void read(QRMatrix& matrix, int& x, int& y);
public:
	QRCode();
	QRCode(DataBuffer* dataBuffer);
	void setBuffer(DataBuffer* dataBuffer);
	bool decode(Mat img);
};

