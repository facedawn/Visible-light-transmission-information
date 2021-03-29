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
#define numberofblock (NumberofColorBlocks + 4)
#define numberofpoint 825

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
	char* ans;
	bool getQR;
	Point2f vertexs_minRect_QR[4]; //记录二维码的四个点

	void SobelEnhance(const Mat src, Mat& dst);
	void CannyThreshold(int, void*);
	void canny(Mat a);



	string int2str(const int& int_temp);
	int find_point(vector<po>& s);
	void find4j();
	bool location2(Mat a);
	void end_correct(Mat f);
	void read(Mat a);

public:
	QRLocation(Mat src);
	QRList get();
};

