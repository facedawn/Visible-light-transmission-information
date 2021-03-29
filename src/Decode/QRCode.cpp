#include "QRCode.h"

char xorMatrix[NumberofColorBlocks][NumberofColorBlocks];

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
	//定位二维码的位置
	QRLocation location(img);
	return location.get();
}

bool QRCode::decode(Mat img)
{
	if (buffer == NULL)
		return false;

	int totalQR = 0; //总计二维码数量
	int nowQR = 0; //当前是第几张二维码
	QRList QRlist = locationQR(img);
	for (int i = 0; i < QRlist.size(); i++)
	{
		//for (int h = 0; h < QRlist[i].height(); h++) 
		//{
		//	for (int w = 0; w < QRlist[i].width(); w++) 
		//	{
		//		printf("%d ", QRlist[i].at(h, w));
		//	}
		//	printf("\n");
		//}
		if (versionCheck(QRlist[i]) && sizeCheck(QRlist[i]) && EffectivenessCheck(QRlist[i]))
		{
			read(QRlist[i]);
		}
	}

	return true;
}
void QRCode::fixPoint(int& x, int& y)
{
	//x为行，y为列
	//自动步进一步，找到可以落笔的空位。一旦溢出就会设为0,0
	x++;
	if (x >= NumberofColorBlocks)
	{
		x = 0;
		y++;
	}
	if (y >= NumberofColorBlocks)
	{
		x = NumberofColorBlocks - 1;
		y = NumberofColorBlocks - 1;
		return;
	}
}

bool QRCode::versionCheck(QRMatrix& matrix)
{
	return true;
}
bool QRCode::sizeCheck(QRMatrix& matrix)
{
	return true;
}
bool QRCode::EffectivenessCheck(QRMatrix& matrix)
{
	return true;
}
void QRCode::read(QRMatrix& matrix)
{

}