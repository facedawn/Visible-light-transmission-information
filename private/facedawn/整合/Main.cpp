#include <opencv2/opencv.hpp>
#include <iostream>
#include "Decode.h"

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
	if (decode.video2Data())
		cout << "打开成功！" << endl;
	else
		cout << "由于未知原因打开失败！" << endl;

	return 0;
}