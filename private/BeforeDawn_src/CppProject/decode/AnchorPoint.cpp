#include "AnchorPoint.h"


AnchorPoint::AnchorPoint()
{
	for (int i = 0; i < 4; i++)
	{
		block[i].reset();
	}
	QR.reset();
}
AnchorPoint::AnchorPoint(BlockCoordinates block1, BlockCoordinates block2, BlockCoordinates block3, BlockCoordinates block4, BlockCoordinates blockQR)
{
	block[0] = block1;
	block[1] = block2;
	block[2] = block3;
	block[3] = block4;
	QR = blockQR;
}

void AnchorPoint::setLeftTop(BlockCoordinates other)
{
	block[0] = other;
}
void AnchorPoint::setLeftTop(Points p1, Points p2)
{
	block[0] = BlockCoordinates(p1, p2);
}
void AnchorPoint::setLeftTop(int x1, int y1, int x2, int y2)
{
	block[0] = BlockCoordinates(x1, y1, x2, y2);
}

void AnchorPoint::setRightTop(BlockCoordinates other)
{
	block[1] = other;
}
void AnchorPoint::setRightTop(Points p1, Points p2)
{
	block[1] = BlockCoordinates(p1, p2);
}
void AnchorPoint::setRightTop(int x1, int y1, int x2, int y2)
{
	block[1] = BlockCoordinates(x1, y1, x2, y2);
}

void AnchorPoint::setLeftBottom(BlockCoordinates other)
{
	block[2] = other;
}
void AnchorPoint::setLeftBottom(Points p1, Points p2)
{
	block[2] = BlockCoordinates(p1, p2);
}
void AnchorPoint::setLeftBottom(int x1, int y1, int x2, int y2)
{
	block[2] = BlockCoordinates(x1, y1, x2, y2);
}

void AnchorPoint::setRightBottom(BlockCoordinates other)
{
	block[3] = other;
}
void AnchorPoint::setRightBottom(Points p1, Points p2)
{
	block[3] = BlockCoordinates(p1, p2);
}
void AnchorPoint::setRightBottom(int x1, int y1, int x2, int y2)
{
	block[3] = BlockCoordinates(x1, y1, x2, y2);
}

void AnchorPoint::setQRPlace(BlockCoordinates other)
{
	QR = other;
}
void AnchorPoint::setQRPlace(Points p1, Points p2)
{
	QR = BlockCoordinates(p1, p2);
}
void AnchorPoint::setQRPlace(int x1, int y1, int x2, int y2)
{
	QR = BlockCoordinates(x1, y1, x2, y2);
}


BlockCoordinates AnchorPoint::leftTop()
{
	return block[0];
}
BlockCoordinates AnchorPoint::rightTop()
{
	return block[1];
}
BlockCoordinates AnchorPoint::leftBottom()
{
	return block[2];
}
BlockCoordinates AnchorPoint::rightBottom()
{
	return block[3];
}
BlockCoordinates AnchorPoint::QRPlace()
{
	return QR;
}