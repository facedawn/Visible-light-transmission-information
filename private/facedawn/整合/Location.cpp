#include "Location.h"
double Location::cross(Point before, Point next, Point mid)//before->mid    mid->next  向量叉乘（平行四边形面积）
{
	double x1, y1, x2, y2;
	x1 = mid.x - before.x;
	x2 = next.x - mid.x;
	y1 = mid.y - before.y;
	y2 = next.y - mid.y;
	return (x1 * y2) - (x2 * y1);
}

double Location::dis(Point a, Point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

bool cmp(Point a, Point b)//根据该点与基准点的连线和水平线所成的夹角排序
{
	double tana, tanb;
	if (a.x - base.x == 0)
		tana = (a.y - base.y) * 1000;//此处默认x差值最小不小于0.001
	else
		tana = (a.y - base.y) / (a.x - base.x);
	if (b.x - base.x == 0)
		tanb = (b.y - base.y) * 1000;
	else
		tanb = (b.y - base.y) / (b.x - base.x);
	if (tana != tanb)
		return tana < tanb;
	else
		return dis(a, base) < dis(b, base);
}

void findbase(vector<Point>& a)
{
	int cnt = a.size(), t = 0;
	base = a[0];
	for (int i = 0; i < cnt; i++)
	{
		if (a[i].x < base.x || (a[i].x == base.x && a[i].y < base.y))
		{
			base = a[i];
			t = i;
		}
	}
	swap(a[t], a[0]);
}

vector<Point> find_tubao(vector<Point>& a)
{
	vector<Point> result;
	//findbase(a);
	//sort(a.begin() + 1, a.end(), cmp);
	result.push_back(a[0]);
	int top = 0, cnt = a.size();
	for (int i = 1; i < cnt; i++)
	{
		while (top > 0 && cross(result[top - 1], a[i], result[top]) >= 0)
		{
			top--;
			result.pop_back();
		}
		result.push_back(a[i]); top++;
	}
	return result;
}

void show(Mat img)
{
	imshow("show", img);
	waitKey();
}

void SobelEnhance(const Mat src, Mat& dst)
{
	Mat sobelx(src.size(), CV_16SC1);///边缘检测后，会有负值，也会有大于255的值，因此类型设为CV_16SC1有符号类型
	Mat sobely(src.size(), CV_16SC1);
	Mat img_edgeFiltex(src.size(), CV_8UC1);///结果图，类型设为CV_8UC1进行阈值截断
	Mat img_edgeFiltey(src.size(), CV_8UC1);
	Sobel(src, sobelx, CV_16SC1, 1, 0, 3);
	convertScaleAbs(sobelx, img_edgeFiltex);
	Sobel(src, sobely, CV_16SC1, 0, 1, 3);
	convertScaleAbs(sobely, img_edgeFiltey);
	///像素加权
	addWeighted(img_edgeFiltex, 1, img_edgeFiltey, 1, 0, dst);
}

void CannyThreshold(int, void*)
{
	/// 使用 3x3内核降噪
	//blur(src_gray, detected_edges, Size(3, 3));//输入灰度图src_gray，输出滤波后的图片到detected_edges
	equalizeHist(detected_edges, detected_edges);//直方图均衡化，增加像素灰度值的动态范围，达到增强图像整体对比度的效果


	GaussianBlur(src_gray, detected_edges, Size(7, 7), 0, 0);
	threshold(src_gray, detected_edges, 150, 255, THRESH_BINARY);


	//SobelEnhance(detected_edges, detected_edges);// 增强图象

	//detected_edges = src_gray;
	//imwrite("pic.jpg", detected_edges);
	//show(detected_edges);
	//threshold(detected_edges, detected_edges, 150, 255, 0);
	/// 运行Canny算子, 边缘记录在原图像
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratioo, kernel_size);

	//膨胀两次，让边缘闭合
	cv::dilate(detected_edges, detected_edges, cv::Mat());//膨胀
	cv::dilate(detected_edges, detected_edges, cv::Mat());//膨胀
	//cv::erode(detected_edges, detected_edges, cv::Mat());//腐蚀

	/// 使用 Canny算子输出边缘作为掩码显示原图像
	dst = Scalar::all(0);

	src.copyTo(dst, detected_edges);
	//imshow(window_name, dst);
	imwrite("edge.jpg", dst);
}

void canny(Mat a)
{
	src = a;
	src.copyTo(fsrc);
	//GaussianBlur(src, src, Size(9, 9), 0, 0);
	/*int col = src.cols, row = src.rows;
	while (col > 900 || row > 900)
	{
		col *= 0.99;
		row *= 0.99;
	}
	resize(src, src, Size(col, row));*/


	dst.create(src.size(), src.type());//创建和img相同大小的图
	if (src.type() != CV_8UC1)
		cvtColor(src, src_gray, COLOR_BGR2GRAY);//转灰度图
	else
		src.copyTo(src_gray);
	//namedWindow(window_name, WINDOW_AUTOSIZE);
	//createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
	CannyThreshold(0, 0);
	waitKey(0);
}

bool po_cmp(po a, po b)
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

int find_point(vector<po>& s)//从候选点中找到三个,返回第一个的序号
{
	int all = s.size();
	int col = fsrc.cols, row = src.rows;
	for (int i = 0; i + 2 < all; i++)
	{
		int p1x, p2x, p3x, p1y, p2y, p3y, mid1x, mid1y, mid2x, mid2y, mid3x, mid3y;
		p1x = s[i].maxx - s[i].minx;
		p2x = s[i + 1].maxx - s[i + 1].minx;
		p3x = s[i + 2].maxx - s[i + 2].minx;
		p1y = s[i].maxy - s[i].miny;
		p2y = s[i + 1].maxy - s[i + 1].miny;
		p3y = s[i + 2].maxy - s[i + 2].miny;
		mid1x = (s[i].maxx + s[i].minx) / 2;
		mid1y = (s[i].maxy + s[i].miny) / 2;
		mid2x = (s[i + 1].maxx + s[i + 1].minx) / 2;
		mid2y = (s[i + 1].maxy + s[i + 1].miny) / 2;
		mid3x = (s[i + 2].maxx + s[i + 2].minx) / 2;
		mid3y = (s[i + 2].maxy + s[i + 2].miny) / 2;
		//printf("%d %d\n", col,row);
		double ab, bc, ac;
		ab = sqrt((mid2x - mid1x) * (mid2x - mid1x) + (mid2y - mid1y) * (mid2y - mid1y));
		bc = sqrt((mid2x - mid3x) * (mid2x - mid3x) + (mid2y - mid3y) * (mid2y - mid3y));
		ac = sqrt((mid3x - mid1x) * (mid3x - mid1x) + (mid3y - mid1y) * (mid3y - mid1y));
		double cosa, cosb, cosc;

		cosa = ((mid2x - mid1x) * (mid3x - mid1x) + (mid2y - mid1y) * (mid3y - mid1y)) / ab / ac;
		cosb = ((mid1x - mid2x) * (mid3x - mid2x) + (mid1y - mid2y) * (mid3y - mid2y)) / ab / bc;
		cosc = ((mid2x - mid3x) * (mid1x - mid3x) + (mid2y - mid3y) * (mid1y - mid3y)) / bc / ac;

		printf("a:%d %d\nb:%d %d\nc:%d %d\n", mid1x, mid1y, mid2x, mid2y, mid3x, mid3y);

		printf("cosa=%lf	cosb=%lf	cosc= %lf\n", cosa, cosb, cosc);
		if (abs(p3x - p1x) <= col / 150 && abs(p3y - p1y) <= row / 150 && abs(p3x - p2x) <= col / 150 && abs(p3y - p2y) <= row / 150)
		{
			//printf("a:%d %d\nb:%d %d\nc:%d %d\n", mid1x, mid1y, mid2x, mid2y, mid3x, mid3y);

			//printf("cosa=%lf	cosb=%lf	cosc= %lf\n", cosa, cosb, cosc);

		}
		else continue;
		double p = 0.25;
		if (cosa<p && cosa>-p || cosb<p && cosb>-p || cosc<p && cosc>-p)
		{//printf("%d..\n", i);
			return i;
			break;
		}
		else continue;
	}
	return -1;
}

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
			swap(vertexs_minRect_QR[i], vertexs_minRect_QR[3]);
		}
		if (vertexs_minRect_QR[i].x < mid.x && vertexs_minRect_QR[i].y > mid.y)
		{
			swap(vertexs_minRect_QR[i], vertexs_minRect_QR[2]);
		}
		if (vertexs_minRect_QR[i].x > mid.x && vertexs_minRect_QR[i].y < mid.y)
		{
			swap(vertexs_minRect_QR[i], vertexs_minRect_QR[0]);
		}
		if (vertexs_minRect_QR[i].x > mid.x && vertexs_minRect_QR[i].y > mid.y)
		{
			swap(vertexs_minRect_QR[i], vertexs_minRect_QR[1]);
		}
	}
	if (ccc == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			vertexs_minRect_QR[i] += (vertexs_minRect_QR[i] - mid) / 10;
		}
	}
}

bool location2(Mat a)
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
		return false;
	}
	int all = vector_contours_filter.size();
	vector<po>s;//存储定位点信息
	/*string start = "out", end = ".jpg";*/
	for (int j = 0; j < all; j++)
	{

		po q;
		q.minx = 1e9, q.miny = 1e9, q.maxx = 0, q.maxy = 0;
		q.i = vector_contours_filter[j];
		q.d = find_tubao(contours[vector_contours_filter[j]]);
		//q.d = contours[vector_contours_filter[j]];
		approxPolyDP(q.d, q.d, 2, true);

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
	sort(s.begin(), s.end(), po_cmp);//排序，找相近的
	int ans = -1;
	ans = find_point(s);//返回三个候选点的第一个的序号
	if (ans == -1)
	{
		printf("not match\n");
		return false;
	}
	vector<Point>res_contours;

	res_contours = contours[hierarchy[s[ans].i][3]];

	//res_contours = find_tubao(res_contours);

	approxPolyDP(res_contours, res_contours, 50, true);
	cout << res_contours << endl;
	cout << res_contours.size() << endl;

	vector<Point2f>point_minRect12;
	for (int i = 0; i < 4; i++)
	{
		Point2f temp = Point2f((double)res_contours[i].x, (double)res_contours[i].y);
		point_minRect12.push_back(temp);
	}
	for (int j = 0; j < 4; j++)
	{
		vertexs_minRect_QR[j] = point_minRect12[j];
		//line(src, point_minRect12[j], point_minRect12[(j + 1) % 4], Scalar(0, 0, 255), 1, 8);
	}

	Point2f midPoint_rect;

	for (int j = 0; j < 4; j++)
	{
		midPoint_rect.x += (point_minRect12[j].x / 4);
		midPoint_rect.y += (point_minRect12[j].y / 4);
	}
	//circle(src, midPoint_rect, 10, Scalar(0, 255, 0), 3, 8);
	//src.copyTo(fsrc);
	int f = 0;
	/*for (int i = 0; i < 4; i++)
	{
		if (vertexs_minRect_QR[i].x > midPoint_rect.x && vertexs_minRect_QR[i].y > midPoint_rect.y)
		{
			f = i;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		vertexs_minRect_QR[i] = point_minRect12[(i + 4 - f)%4];
	}*/
	find4j();

	f = 6;
	for (int i = ans; i < ans + 3; i++)
	{
		if (s[i].minx < midPoint_rect.x && s[i].miny < midPoint_rect.y)
		{
			f -= 2;
		}
		if (s[i].minx > midPoint_rect.x && s[i].miny < midPoint_rect.y)
		{
			f -= 3;
		}
		if (s[i].minx > midPoint_rect.x && s[i].miny > midPoint_rect.y)
		{
			f -= 0;
		}
		if (s[i].minx < midPoint_rect.x && s[i].miny > midPoint_rect.y)
		{
			f -= 1;
		}
	}
	Point2f temp1 = vertexs_minRect_QR[(2 + 0 + f) % 4];
	Point2f temp2 = vertexs_minRect_QR[(3 + 0 + f) % 4];
	Point2f temp3 = vertexs_minRect_QR[(0 + 0 + f) % 4];
	Point2f temp0 = vertexs_minRect_QR[(1 + 0 + f) % 4];

	vertexs_minRect_QR[0] = temp0;
	vertexs_minRect_QR[1] = temp1;
	vertexs_minRect_QR[2] = temp2;
	vertexs_minRect_QR[3] = temp3;

	int col = src.cols, row = src.rows;
	while (col > 1000 || row > 1000)
	{
		col *= 0.99;
		row *= 0.99;
	}
	resize(src, src, Size(col, row));

	//show(src);

}

void end_correct(Mat f)
{
	int lenth = 826;
	Point2f vertex_warp[4];
	vertex_warp[1] = Point2f(0, float(lenth - 1));
	vertex_warp[2] = Point2f(0, 0);
	vertex_warp[3] = Point2f(float(lenth - 1), 0);
	vertex_warp[0] = Point2f(float(lenth - 1), float(lenth));
	//find4j();
	Mat transform = getPerspectiveTransform(vertexs_minRect_QR, vertex_warp);
	//show(f);
	warpPerspective(f, dst, transform, Size(lenth, lenth));//dst中为初次切割下来的二维码
}


void read(Mat a)
{
	cvtColor(a, a, COLOR_RGB2GRAY);
	//show(a);
	int pointOfeachBlock = numberofpoint / numberofblock;
	for (int i = 0; i < numberofblock; i++)
	{
		for (int j = 0; j < numberofblock; j++)
		{

			circle(a, Point2f(i * pointOfeachBlock, j * pointOfeachBlock), 1, Scalar(255, 0, 0));
			int cnt = 0;
			for (int ii = i * pointOfeachBlock; ii < (i + 1) * pointOfeachBlock; ii++)
			{
				uchar* data = a.ptr<uchar>(ii);
				for (int jj = j * pointOfeachBlock; jj < (j + 1) * pointOfeachBlock; jj++)
				{
					if (data[jj] < 150)
						cnt++;//记录黑像素点的个数
				}
			}
			//printf("%d %d: %d\n", i, j, cnt);
			if ((double)cnt / (pointOfeachBlock * pointOfeachBlock) > 0.5)//黑块多
			{
				//printf("%d ", cnt);
				ans[i][j] = 1;
			}
			else
			{
				ans[i][j] = 0;
			}
			//fprintf(fp, "%d ", ans[i][j]);
		}
	}
}

Location::Location(Mat a)
{
	source = a;
	if (src.empty()) {
		//检查是否读取图像
		cout << "Error! Input image cannot be read...\n";
		return -1;
	}
	if (location2(src))
	{
		end_correct(fsrc);
	}
	read(dst);
	res.matrix = ans;
	res.w = numberofblock;
	res.h = numberofblock;
}