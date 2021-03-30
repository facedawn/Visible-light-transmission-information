#pragma once
#include <opencv2/opencv.hpp>
#include <cstdio>
#include <random>
#include <vector>
#include <algorithm>
#include <ctime>
#include "QRMatrix.h"
#include "QRList.h"
#include "GlobalMacroDefine.h"
#define blockNumberOfWhite 2 
#define numberofblock (NumberofColorBlocks + 2*blockNumberOfWhite)
#define numberofpoint numberofblock*4


using namespace cv;
using namespace std;
struct po
{
	int minx, maxx, miny, maxy;
	int i;
	vector<Point> d;
};

class QRLocation
{
	Mat QRLocation_src, QRLocation_src_gray;
	Mat QRLocation_dst, detected_edges;
	Mat QRLocation_fsrc;
	char* ans;
	bool getQR;
	Point2f vertexs_minRect_QR[4]; //记录二维码的四个点

	//边缘增强函数
	void SobelEnhance(const Mat src, Mat& dst);


	//拷贝，转灰度图，降噪，并调用openCV自带的Canny函数获取边缘
	//膨胀操作用于帮助边缘闭合，利于之后的轮廓提取
	void canny(Mat a);

	string int2str(const int& int_temp);

	//s中保存有全部的候选定位点，根据三个定位点的夹角和面积特征进行筛选
	//从候选点中找到三个,返回第一个的序号i，三个定位点序号i，i+1，i+2
	int find_point(vector<po>& s);

	//按照顺时针方向摆好四个顶点
	void find4j();

	//定位主函数，输入带有二维码的Mat，返回是否找到二维码。
	//若是找到，则将二维码的四个顶点保存在vertexs_minRect_QR数组中，便于截取矫正
	bool location2(Mat a);

	//矫正函数，输入图片，根据location函数中获得的四顶点进行截取和矫正
	//最终获得矫正后的二维码，存放在QRLocation_dst中
	void end_correct(Mat f);

	//输入矫正后的图像，根据像素数以及二维码内块数进行均分
	//均分后遍历块内的像素点，判断黑白哪一方较多，用较多的一方代替这一块的颜色
	void read(Mat a);

public:

	//构造函数，输入一张图片，进行定位矫正和读取，结果存在public的ans中
	QRLocation(Mat src);

	//public函数，提供接口给外界，返回一个QRList
	QRList get();
};

