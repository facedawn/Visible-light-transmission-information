#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>
#include <vector>
using namespace cv;
using namespace std;
#define NumberofColorBlocks 200
#define Numbmerofpixels 3
// 暂定每边200个色块，每个色块3个像素，一边共600个像素
// 提供一个对应于图片黑白色块的01矩阵，生成相对应的图片
int matrix[NumberofColorBlocks+10][NumberofColorBlocks+10];
vector<Vec3b> rgb_pixels;  //创建 rgb像素点列表

int main()
{
	rgb_pixels.push_back({ 255,255,255 });//0--白色
	rgb_pixels.push_back({0,0,0 });//1--黑色
	int all = NumberofColorBlocks * Numbmerofpixels;
	Mat img(all, all, CV_8UC3, Scalar(0, 0, 0));
	int i, j, p, q;
	for (i = 0; i < NumberofColorBlocks; i++)
	{
		for (j = 0; j < NumberofColorBlocks; j++)
		{
			matrix[i][j] = -1;
		}
	}//全部标上-1，代表未使用
	int bigAnchorPoint[8][8] = {
		1,1,1,1,1,1,1,0,
		1,0,0,0,0,0,1,0,
		1,0,1,1,1,0,1,0,
		1,0,1,0,1,0,1,0,
		1,0,1,1,1,0,1,0,
		1,0,0,0,0,0,1,0,
		1,1,1,1,1,1,1,0,
		0,0,0,0,0,0,0,0
	};//大定位点
	int smallAnchorPoint[5][5] = {
		1,1,1,1,1,
		1,0,0,0,1,
		1,0,1,0,1,
		1,0,0,0,1,
		1,1,1,1,1
	};//小定位点
	for (i = 0; i < NumberofColorBlocks; i++)
	{
		for (j = 0; j < NumberofColorBlocks; j++)
		{
			if(matrix[i][j] == -1);
			matrix[i][j] = rand() % 2;
		}
	}//用于存储01信息的部分，共能存 NumberofColorBlocks*Numbmerofpixels-217 个bit

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			matrix[i][j] = bigAnchorPoint[i][j];
			matrix[NumberofColorBlocks - 1 - i][j] = bigAnchorPoint[i][j];
			matrix[i][NumberofColorBlocks - 1 - j] = bigAnchorPoint[i][j];
		}
	}
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			matrix[NumberofColorBlocks - 1 - i][NumberofColorBlocks - 1 - j] = smallAnchorPoint[i][j];
		}
	}
	for (i = 0; i*Numbmerofpixels < img.rows; i++)
	{
		for (j = 0; j*Numbmerofpixels < img.cols; j++)
		{
			for (p = 0; p < Numbmerofpixels; p++)
			{
				for (q = 0; q < Numbmerofpixels; q++)
				{	
					img.at<Vec3b>(i*Numbmerofpixels+p, j*Numbmerofpixels+q) =rgb_pixels[matrix[i][j]];
				}
			}
		}
	}
	imshow("img", img);
	waitKey();
}

