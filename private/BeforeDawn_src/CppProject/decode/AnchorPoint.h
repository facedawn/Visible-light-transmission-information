#pragma once
#include "BlockCoordinates.h"
#include "Points.h"

class AnchorPoint
{
	BlockCoordinates block[4]; //四个定位点的坐标（从左上开始，顺时针）
	BlockCoordinates QR; //二维码图片的位置
public:
	AnchorPoint();
	AnchorPoint(BlockCoordinates block1, BlockCoordinates block2, BlockCoordinates block3, BlockCoordinates block4, BlockCoordinates blockQR);;

	void setLeftTop(BlockCoordinates other);
	void setLeftTop(Points p1, Points p2);
	void setLeftTop(int x1, int y1, int x2, int y2);
	void setRightTop(BlockCoordinates other);
	void setRightTop(Points p1, Points p2);
	void setRightTop(int x1, int y1, int x2, int y2);
	void setLeftBottom(BlockCoordinates other);
	void setLeftBottom(Points p1, Points p2);
	void setLeftBottom(int x1, int y1, int x2, int y2);
	void setRightBottom(BlockCoordinates other);
	void setRightBottom(Points p1, Points p2);
	void setRightBottom(int x1, int y1, int x2, int y2);

	void setQRPlace(BlockCoordinates other);
	void setQRPlace(Points p1, Points p2);
	void setQRPlace(int x1, int y1, int x2, int y2);

	BlockCoordinates leftTop();
	BlockCoordinates rightTop();
	BlockCoordinates leftBottom();
	BlockCoordinates rightBottom();
	BlockCoordinates QRPlace();
};

