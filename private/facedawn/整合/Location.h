#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <ctime>
#include "QRMatrix.h"
#include "QRList.h"
#define numberofblock 55
#define numberofpoint 825
using namespace cv;
using namespace std;
struct po
	{
		int minx, maxx, miny, maxy;
		int i;
		vector<Point> d;
	};

class Location
{
public:
	
	Location(Mat src);
	QRList get()
	{
		QRList list;
		QRMatrix matrix(ans, numberofblock, numberofblock);
		list.append(matrix);
		return list;
	}

private:
	char* ans;

	Point2f vertexs_minRect_QR[4];//记录二维码四个顶点的

	void SobelEnhance(const Mat src, Mat& dst);

	void CannyThreshold(int, void*);

	void canny(Mat a);

	

	string int2str(const int& int_temp);

	int find_point(vector<po>& s);

	void find4j();

	bool location2(Mat a);

	void end_correct(Mat f);

	void read(Mat a);

};