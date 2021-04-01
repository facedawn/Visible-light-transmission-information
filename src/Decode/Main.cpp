#include <opencv2/opencv.hpp>
#include <iostream>
#include "Decode.h"
#include <ctime>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	cout << "请输入视频文件地址：";
	string video;
	cin >> video;
	cout << "请输入输出文件地址：";
	string output;
	cin >> output;
	Decode decode(output, video);

	//===========================
	double second;
	clock_t s_time, e_time;
	s_time = clock();

	int size = decode.video2Data(); //核心

	e_time = clock();
	second = (double)(e_time - s_time) / CLOCKS_PER_SEC;
	//============================
	if (size)
	{
		cout << "读取完成！" << endl;

		printf("\nrun time:%.4fs\n", second);
		printf("total byte:%d\n", size);
		double bps = (double)(size * 8) / second;
		printf("speed:%lf Mbps\n", bps / 1000000);
	}
	else
		cout << "由于未知原因打开失败！" << endl;

	string tmp;
	cin >> tmp; //暂停
	return 0;
}