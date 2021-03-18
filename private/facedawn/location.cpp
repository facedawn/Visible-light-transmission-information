#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>
#include <vector>
#include "find_tb.h"
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
Mat fsrc;	

/**
 * @函数 CannyThreshold
 * @简介： trackbar 交互回调 - Canny阈值输入比例1:3
 */
void CannyThreshold(int, void*)
{
	/// 使用 3x3内核降噪
	blur(src_gray, detected_edges, Size(3, 3));//输入灰度图src_gray，输出滤波后的图片到detected_edges
	GaussianBlur(src_gray, detected_edges, Size(7, 7), 0, 0);
	
	equalizeHist(detected_edges, detected_edges);//直方图均衡化，增加像素灰度值的动态范围，达到增强图像整体对比度的效果
	//threshold(detected_edges, detected_edges, 150, 255, 0);
	/// 运行Canny算子, 边缘记录在原图像
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratioo, kernel_size);

	/// 使用 Canny算子输出边缘作为掩码显示原图像
	dst = Scalar::all(0);

	src.copyTo(dst, detected_edges);
	imshow(window_name, dst);imwrite("out11.jpg", dst);
}

/**
 * @函数 canny
 * @简介： 输入一个Mat，获得该图边缘
 */
void canny(Mat a)
{
	src = a;
	GaussianBlur(src, src, Size(9, 9), 0, 0);

	int col = src.cols, row = src.rows;
	while (col > 1500 || row > 1500)
	{
		col *= 0.99;
		row *= 0.99;
	}
	resize(src, src, Size(col, row));
	src.copyTo(fsrc);

	dst.create(src.size(), src.type());//创建和img相同大小的图
	cvtColor(src, src_gray ,COLOR_BGR2GRAY);//转灰度图
	show(src_gray);
	namedWindow(window_name, WINDOW_AUTOSIZE);
	//createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
	CannyThreshold(0, 0);
	waitKey(0);
}

struct po
{
	int minx, maxx, miny, maxy;
	vector<Point> d;
};

bool po_cmp(po a,po b)
{
	if (a.maxx - a.minx != b.maxx - b.minx)
	{
		return a.maxx - a.minx > b.maxx - b.minx;
	}
	else
		return a.maxy - a.miny > b.maxy - b.miny;
}//对候选定位点进行排序

string int2str(const int& int_temp)
{
	string string_temp;
	stringstream stream;
	stream << int_temp;
	string_temp = stream.str();   //此处也可以用 stream>>string_temp  
	return string_temp;
}

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
	findContours(detected_edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<int> vector_contours_filter;//其中存放定位点的候选
	for (int k, c, i = 0; i < contours.size(); i++)
	{
		k = i;
		c = 0;
		while (hierarchy[k][2] != -1)//表示有子轮廓
		{
			k = hierarchy[k][2];
			c = c + 1;
		}

		//----------------------Index到此一游（c >= 5 -> c >= 4）-------------------------//

		if (c >= 4)			//有大于等于5层子轮廓的，视为定位点候选
		{
			vector_contours_filter.push_back(i);
		}
	}


	vector<Point> vertex_minRect4;//装有三个定位点共12个顶点的容器
	if ((vector_contours_filter.size() < 3))//如果没有找到足够的定位点
	{
		printf("The picture does not meet the requirements\n");
		printf("cnt=	%d\n", vector_contours_filter.size());
		return;
	}
	int all = vector_contours_filter.size();
	printf("all point: %d\n", all);
	vector<po>s;//存储定位点信息
	/*string start = "out", end = ".jpg";*/
	for (int j = 0; j < all; j++)
	{
		
		po q;
		q.minx = 1e9, q.miny = 1e9, q.maxx = 0, q.maxy = 0;
		q.d = find_tubao(contours[vector_contours_filter[j]]);
		//q.d = contours[vector_contours_filter[j]];
		int cnt = q.d.size();
		if (cnt < 4)continue;
		for (int i = 0; i < cnt; i++)//把确定下来的矩形顶点丢进去
		{
			//int temp = contours[vector_contours_filter[0]].size() / 4;
			q.minx = min(q.minx, q.d[i].x);
			q.miny = min(q.miny, q.d[i].y);
			q.maxx = max(q.maxx, q.d[i].x);
			q.maxy = max(q.maxy, q.d[i].y);
			//printf("%d %d\n", contours[vector_contours_filter[j]][i].x, contours[vector_contours_filter[j]][i].y);
		}
		//printf("\n%d\n", vector_contours_filter[0]);
		/*Mat temp;
		temp = src(Range(q.miny, q.maxy), Range(q.minx, q.maxx));
		string path = start + int2str(j) + end;
		imwrite(path, temp);*/
		//show(temp);

		
		//q.d = contours[vector_contours_filter[j]];
		s.push_back(q);
	}
	printf("all : %d\n", all);
	sort(s.begin(), s.end(), po_cmp);//排序，找相近的
	int ans = -1;
	if (all > 3)
	{
		for (int i = 0; i + 2 < all; i++)
		{
			int p1x, p2x, p3x, p1y, p2y, p3y;
			p1x = s[i].maxx - s[i].minx;
			p2x = s[i + 1].maxx - s[i + 1].minx;
			p3x = s[i + 2].maxx - s[i + 2].minx;
			p1y = s[i].maxy - s[i].miny;
			p2y = s[i + 1].maxy - s[i + 1].miny;
			p3y = s[i + 2].maxy - s[i + 2].miny;
			if (abs(p3x - p1x) <= 6 && abs(p3y - p1y) <= 6&& abs(p3x - p2x) <= 6 && abs(p3y - p2y) <= 6)
			{
				printf("%d %d %d %d\n",s[i+1].maxx,s[i+1].minx, s[i + 1].maxy, s[i + 1].miny);
				ans = i;
				break;
			}
		}

		if (ans == -1)
		{
			printf("not match\n");
			return;
		}
	}
	else
		ans = 0;
	printf("ans:%d\n", ans);
	for (int i=ans;i<ans+3;i++)
	{
		//cout << "cnt:	" << i << " minx= " << s[i].minx << " maxx= " << s[i].maxx << " miny= " << s[i].miny << " maxy= " << s[i].maxy << endl;

		int cnt = s[i].d.size(); printf("cnt: %d\n", cnt);
		for (int j = 0; j < cnt; j++)
		{
			line(src, s[i].d[j], s[i].d[(j+1)%cnt], Scalar(0, 255, 0), 1, 8);
			vertex_minRect4.push_back(s[i].d[j]);
			//printf("%d %d\n", s[i].d[j].x, s[i].d[j].y);
		}
	}
	Point P1, P2; ll maxdis = 0;
	int cnt = vertex_minRect4.size(); 
	for (int yi = 0; yi < cnt; yi++)//找到距离最远两点
	{
		for (int yt = yi + 1; yt < cnt; yt++)
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
	//距离最远两点的中点为二维码的中点
	for (int yi = 0; yi < cnt; yi++)
	{
		Point temp;
		temp.x = (P1.x + P2.x) - vertex_minRect4[yi].x;
		temp.y = (P1.y + P2.y) - vertex_minRect4[yi].y;
		vertex_minRect4.push_back(temp);
	}//根据中点，全部对称过去，可以得到第四个角的位置
	
	Point2f point_minRect12[4];

	RotatedRect rect12 = minAreaRect(vertex_minRect4);//画最小包围矩阵
	//vertex_minRect4中存放定位点的全部顶点

	rect12.points(point_minRect12);
	for (int j = 0; j < 4; j++)
	{
		vertexs_minRect_QR[j] = point_minRect12[j];
		line(src, point_minRect12[j], point_minRect12[(j + 1) % 4], Scalar(0, 0, 255), 1, 8);
	}
	
	Point2f midPoint_rect;
	for (int j = 0; j < 4; j++)
	{
		midPoint_rect.x += (point_minRect12[j].x / 4);
		midPoint_rect.y += (point_minRect12[j].y / 4);
	}
	circle(src, midPoint_rect, 10, Scalar(0, 255, 0), 3, 8);
	src.copyTo(fsrc);

	int col = src.cols, row = src.rows;
	while (col > 1000 || row > 1000)
	{
		col *= 0.99;
		row *= 0.99;
	}
	resize(src, src, Size(col, row));
	
	show(src);
}

//把矩形四角和窗口四角对应
void find4j()
{
	Point2f mid = Point2f(0, 0);
	for (int i = 0; i < 4; i++)
	{
		mid.x += vertexs_minRect_QR[i].x / 4;
		mid.y += vertexs_minRect_QR[i].y / 4;
	}
	for (int i = 0; i < 4; i++)
	{
		if (vertexs_minRect_QR[i].x < mid.x && vertexs_minRect_QR[i].y < mid.y)
		{
			swap(vertexs_minRect_QR[i], vertexs_minRect_QR[2]);
		}
		if (vertexs_minRect_QR[i].x < mid.x && vertexs_minRect_QR[i].y > mid.y)
		{
			swap(vertexs_minRect_QR[i], vertexs_minRect_QR[1]);
		}
		if (vertexs_minRect_QR[i].x > mid.x && vertexs_minRect_QR[i].y < mid.y)
		{
			swap(vertexs_minRect_QR[i], vertexs_minRect_QR[3]);
		}
		if (vertexs_minRect_QR[i].x > mid.x && vertexs_minRect_QR[i].y > mid.y)
		{
			swap(vertexs_minRect_QR[i], vertexs_minRect_QR[0]);
		}
	}
}

void correct()
{
	//int lenth =1+ sqrt((vertexs_minRect_QR[0].x - vertexs_minRect_QR[1].x)* (vertexs_minRect_QR[0].x - vertexs_minRect_QR[1].x)+ (vertexs_minRect_QR[0].y - vertexs_minRect_QR[1].y)+ (vertexs_minRect_QR[0].y - vertexs_minRect_QR[1].y));
	int lenth = 500;
	Point2f vertex_warp[4];
	vertex_warp[1] = Point2f(0, float(lenth - 1));
	vertex_warp[2] = Point2f(0, 0);
	vertex_warp[3] = Point2f(float(lenth - 1), 0);
	vertex_warp[0] = Point2f(float(lenth - 1), float(lenth));
	find4j();
	Mat transform = getPerspectiveTransform(vertexs_minRect_QR, vertex_warp);
	warpPerspective(fsrc, dst, transform, Size(lenth, lenth));
	//show(fsrc);
	show(dst);
	imwrite("out.jpg", dst);
}

int main()
{
	//src=pic();
	src = imread("C:\\Users\\xc\\Desktop\\1616081160493.jpg", 1);
	//src = imread("out.jpg");
	//canny(src);
	if (src.empty()) {
		//检查是否读取图像
		cout << "Error! Input image cannot be read...\n";
		return -1;
	}

	location(src);
	correct();
}
