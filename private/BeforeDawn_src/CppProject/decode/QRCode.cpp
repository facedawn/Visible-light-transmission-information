#include "QRCode.h"

QRCode::QRCode()
{
	buffer = NULL;
}
QRCode::QRCode(DataBuffer* dataBuffer)
{
	setBuffer(dataBuffer);
}
void QRCode::setBuffer(DataBuffer* dataBuffer)
{
	buffer = dataBuffer;
}


QRList QRCode::locationQR(Mat img)
{
	//定位二维码的位置（未完成）
	QRList test;
	BlockCoordinates leftTop(0, 0, 0, 0);
	BlockCoordinates rightTop(0, 0, 0, 0);
	BlockCoordinates leftBottom(0, 0, 0, 0);
	BlockCoordinates rightBottom(0, 0, 0, 0);
	BlockCoordinates QRPlace(0, 0, img.cols, img.rows);
	AnchorPoint tmp(leftTop, rightTop, leftBottom, rightBottom, QRPlace, img);
	//AnchorPoint tmp(BlockCoordinates(), BlockCoordinates(), BlockCoordinates(), BlockCoordinates(), BlockCoordinates(), img);
	test.append(tmp);
	return test;
}
bool QRCode::decode(Mat img)
{
	if (buffer == NULL)
		return false;

	QRList QRlist = locationQR(img);
	for (int i = 0; i < QRlist.size(); i++)
	{

	}

	return true;
}