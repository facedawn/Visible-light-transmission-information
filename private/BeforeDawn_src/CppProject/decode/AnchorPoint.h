#pragma once

#include <opencv2/opencv.hpp>
#include "BlockCoordinates.h"
#include "Points.h"

using namespace cv;

class AnchorPoint
{
	BlockCoordinates block[4]; //四个定位点的坐标（从左上开始，顺时针）（对应裁剪出来的图片）（单位：像素）
	BlockCoordinates QR; //二维码图片的位置（对应裁剪出来的图片）（单位：像素）
	Mat img; //裁剪出来的图片
public:
	AnchorPoint();
	AnchorPoint(Mat tmp);
	AnchorPoint(BlockCoordinates block1, BlockCoordinates block2, BlockCoordinates block3, BlockCoordinates block4, BlockCoordinates blockQR);
	AnchorPoint(BlockCoordinates block1, BlockCoordinates block2, BlockCoordinates block3, BlockCoordinates block4, BlockCoordinates blockQR, Mat tmp);


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

	void setImg(Mat tmp);

	BlockCoordinates leftTop();
	BlockCoordinates rightTop();
	BlockCoordinates leftBottom();
	BlockCoordinates rightBottom();
	BlockCoordinates QRPlace();
	Mat getImg();
};

