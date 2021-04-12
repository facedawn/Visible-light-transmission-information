#include <opencv2/opencv.hpp>
#include <iostream>
#include "Decode.h"
#include <ctime>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	string video;
	string output;
	string correct;
	if (argc == 1)
	{
		cout << "请输入视频文件地址：";
		cin >> video;
		cout << "请输入输出文件地址：";
		cin >> output;
		cout << "请输入输出矫正文件地址：";
		cin >> correct;
	}
	else if (argc == 4)
	{
		video = argv[1];
		output = argv[2];
		correct = argv[3];
	}
	else
	{
		cout << "参数数量不正确！请检查！" << endl;
		return 0;
	}
	Decode decode(video, output, correct);

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
	if (argc == 1)
	{
		cout << endl << "输入任意字符结束程序..." << endl;
		string tmp;
		cin >> tmp; //暂停
	}
	return 0;
}