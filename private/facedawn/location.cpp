#include <opencv2/opencv.hpp>
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <random>
#include <vector>
#define ll long long
using namespace cv;
using namespace std;
#define NumberofColorBlocks 100
#define Numbmerofpixels 5
// 暂定每边200个色块，每个色块3个像素，一边共600个像素
// 提供一个对应于图片黑白色块的01矩阵，生成相对应的图片

Mat pic()
{	
	int matrix[NumberofColorBlocks+10][NumberofColorBlocks+10];
	vector<Vec3b> rgb_pixels;  //创建 rgb像素点列表
	rgb_pixels.push_back({ 255,255,255 });//0--白色
	rgb_pixels.push_back({ 0,0,0 });//1--黑色
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
			if (matrix[i][j] == -1)
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
	for (i = 0; i * Numbmerofpixels < img.rows; i++)
	{
		for (j = 0; j * Numbmerofpixels < img.cols; j++)
		{
			for (p = 0; p < Numbmerofpixels; p++)
			{
				for (q = 0; q < Numbmerofpixels; q++)
				{
					img.at<Vec3b>(i * Numbmerofpixels + p, j * Numbmerofpixels + q) = rgb_pixels[matrix[i][j]];
				}
			}
		}
	}
	imshow("img", img);
	waitKey();
	return img;
}

void show(Mat img)
{
	imshow("show", img);
	waitKey();
}

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold=40;
int const max_lowThreshold = 100;
int ratioo = 3;
int kernel_size = 3;
char* window_name = (char*)"Edge Map";

/**
 * @函数 CannyThreshold
 * @简介： trackbar 交互回调 - Canny阈值输入比例1:3
 */
void CannyThreshold(int, void*)
{
	/// 使用 3x3内核降噪
	blur(src_gray, detected_edges, Size(3, 3));
	equalizeHist(src_gray, src_gray);
	/// 运行Canny算子
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratioo, kernel_size);

	/// 使用 Canny算子输出边缘作为掩码显示原图像
	dst = Scalar::all(0);

	src.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}

/**
 * @函数 canny
 * @简介： 输入一个Mat，获得该图边缘
 */
void canny(Mat a)
{
	src = a;
	dst.create(src.size(), src.type());//创建和img相同大小的图
	cvtColor(src, src_gray ,COLOR_BGR2GRAY);//转灰度图
	namedWindow(window_name, WINDOW_AUTOSIZE);
	//createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
	CannyThreshold(0, 0);
	waitKey(0);
}

struct po
{
	int minx, maxx, miny, maxy;
};

bool cmp(po a,po b)
{
	if (a.maxx - a.minx != b.maxx - b.minx)
	{
		return a.maxx - a.minx > b.maxx - b.minx;
	}
	else
		return a.maxy - a.miny > b.maxy - b.miny;
}//对候选定位点进行排序

Point2f vertexs_minRect_QR[4];//记录二维码四个顶点的
void location(Mat a)
{
	canny(a);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//hierarchy为2维的，hierarchy[i][k]代表第i个轮廓的第k个索引，对应contours中的序号，不存在则为-1
	//	k==0--该轮廓同级的后一个轮廓
	//	k==1--该轮廓同级的前一个轮廓
	//	k==2--该轮廓第一个子轮廓
	//	k==3--该轮廓的父轮廓
	//输入图像，轮廓（点向量形式），轮廓数量，轮廓检索模式，轮廓逼近 
	findContours(detected_edges, contours, hierarchy, RETR_TREE,CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<int> vector_contours_filter;
	for (int k, c, i = 0; i < contours.size(); i++)
	{
		k = i;
		c = 0;
		while (hierarchy[k][2] != -1)//表示不是最外面的轮廓
		{
			k = hierarchy[k][2];
			c = c + 1;
		}
		if (c >= 5)
		{
			vector_contours_filter.push_back(i);
		}
	}
	
	vector<Point> vertex_minRect4;//装有三个定位点共12个顶点的容器
	if ((vector_contours_filter.size() < 3))//如果没有找到足够的定位点
	{
		printf( "The picture does not meet the requirements\n");
		printf("cnt=	%d\n", vector_contours_filter.size());
		return;
	}
	int all = vector_contours_filter.size();
	printf("all point: %d\n", all);
	vector<po>s;//存储定位点信息
	for (int j = 0; j < all; j++)
	{
		int cnt = contours[vector_contours_filter[j]].size();
		po q; 
		q.minx = 1e9, q.miny = 1e9, q.maxx = 0, q.maxy = 0;
		for (int i = 0; i < cnt; i++)//把确定下来的矩形顶点丢进去
		{
			//int temp = contours[vector_contours_filter[0]].size() / 4;
			q.minx = min(q.minx, contours[vector_contours_filter[j]][i].x);
			q.miny = min(q.miny, contours[vector_contours_filter[j]][i].y);
			q.maxx = max(q.maxx, contours[vector_contours_filter[j]][i].x);
			q.maxy = max(q.maxy, contours[vector_contours_filter[j]][i].y);
			
		}//printf("\n%d\n", vector_contours_filter[0]);
		s.push_back(q);
	}
	sort(s.begin(), s.end(), cmp);//排序，找相近的
	int ans = -1;
	for (int i = 0; i+2 < all; i++)
	{
		int p1x, p2x, p3x, p1y, p2y, p3y;
		p1x = s[i].maxx - s[i].minx;
		p2x = s[i + 1].maxx - s[i + 1].minx;
		p3x = s[i + 2].maxx - s[i + 2].minx;
		p1y = s[i].maxy - s[i].miny;
		p2y = s[i + 1].maxy - s[i + 1].miny;
		p3y = s[i + 2].maxy - s[i + 2].miny;
		if (abs(p3x - p1x) <= 3 && abs(p3y - p1y) <= 3)
		{
			ans = i;
			break;
		}
	}
	if (ans == -1)
	{
		printf("not match\n");
		return;
	}
	for (int i=ans;i<ans+3;i++)
	{
		cout << "cnt:	" << i << " minx= " << s[i].minx << " maxx= " << s[i].maxx << " miny= " << s[i].miny << " maxy= " << s[i].maxy << endl;
		Point temp1 = Point(s[i].minx, s[i].miny);Point temp2 = Point(s[i].maxx, s[i].miny);Point temp3 = Point(s[i].minx, s[i].maxy);Point temp4 = Point(s[i].maxx, s[i].maxy);
		vertex_minRect4.push_back(temp1);
		vertex_minRect4.push_back(temp2);
		vertex_minRect4.push_back(temp3);
		vertex_minRect4.push_back(temp4);
		line(src, temp1, temp2, Scalar(0, 255, 0), 3, 8);
		line(src, temp2, temp4, Scalar(0, 255, 0), 3, 8);
		line(src, temp4, temp3, Scalar(0, 255, 0), 3, 8);
		line(src, temp3, temp1, Scalar(0, 255, 0), 3, 8);
	}
	Point P1, P2; ll maxdis = 0;
	for (int yi = 0; yi < 12; yi++)//找到距离最远两点
	{
		for (int yt = yi + 1; yt < 12; yt++)
		{
			ll te = (vertex_minRect4[yi].x - vertex_minRect4[yt].x) * (vertex_minRect4[yi].x - vertex_minRect4[yt].x) + (vertex_minRect4[yi].y - vertex_minRect4[yt].y) * (vertex_minRect4[yi].y - vertex_minRect4[yt].y);
			if (te > maxdis)
			{
				maxdis = te;
				P1 = vertex_minRect4[yi];
				P2 = vertex_minRect4[yt];
			}
		}
	}
	Point mid;
	mid.x = (P1.x + P2.x) / 2;
	mid.y = (P1.y + P2.y) / 2;//距离最远两点的中点为二维码的中点
	for (int yi = 0; yi < 12; yi++)
	{
		Point temp;
		temp.x = mid.x * 2 - vertex_minRect4[yi].x;
		temp.y = mid.y * 2 - vertex_minRect4[yi].y;
		vertex_minRect4.push_back(temp);
	}//根据中点，全部对称过去，可以得到第四个角的位置
	Point2f point_minRect12[4];
	RotatedRect rect12 = minAreaRect(vertex_minRect4);//画最小包围矩阵
	rect12.points(point_minRect12);
	for (int j = 0; j < 4; j++)
	{
		vertexs_minRect_QR[j] = point_minRect12[j];
		line(src, point_minRect12[j], point_minRect12[(j + 1) % 4], Scalar(0, 0, 255), 3, 8);
		line(src, contours[vector_contours_filter[0]][j], contours[vector_contours_filter[0]][(j + 1) % 4], Scalar(0, 255, 0), 3, 8);
		line(src, contours[vector_contours_filter[1]][j], contours[vector_contours_filter[1]][(j + 1) % 4], Scalar(0, 255, 0), 3, 8);
		line(src, contours[vector_contours_filter[2]][j], contours[vector_contours_filter[2]][(j + 1) % 4], Scalar(0, 255, 0), 3, 8);
	}
	
	Point2f midPoint_rect;
	for (int j = 0; j < 4; j++)
	{
		midPoint_rect.x += (point_minRect12[j].x / 4);
		midPoint_rect.y += (point_minRect12[j].y / 4);
	}
	circle(src, midPoint_rect, 10, Scalar(0, 255, 0), 3, 8);
	show(src);
}
Mat fsrc;
void correct()
{
	//int lenth =1+ sqrt((vertexs_minRect_QR[0].x - vertexs_minRect_QR[1].x)* (vertexs_minRect_QR[0].x - vertexs_minRect_QR[1].x)+ (vertexs_minRect_QR[0].y - vertexs_minRect_QR[1].y)+ (vertexs_minRect_QR[0].y - vertexs_minRect_QR[1].y));
	int lenth = 500;
	Point2f vertex_warp[4];
	vertex_warp[1] = Point2f(0, float(lenth - 1));
	vertex_warp[2] = Point2f(0, 0);
	vertex_warp[3] = Point2f(float(lenth - 1), 0);
	vertex_warp[0] = Point2f(float(lenth - 1), float(lenth));
	Mat transform = getPerspectiveTransform(vertexs_minRect_QR, vertex_warp);
	warpPerspective(fsrc, dst, transform, Size(lenth, lenth));
	//show(fsrc);
	show(dst);
	imwrite("out.jpg", dst);
}

int main()
{
	//src=pic();
	src = imread("C:\\Users\\xc\\Desktop\\QQ截图20210316174347.png", 1);
	//src = imread("out.jpg", 1);
	src.copyTo(fsrc);
	//canny(src);
	if (src.empty()) {
		//检查是否读取图像
		cout << "Error! Input image cannot be read...\n";
		return -1;
	}
	location(src);
	correct();
}
