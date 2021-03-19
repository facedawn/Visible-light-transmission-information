#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>
#include <vector>
#include "Buffer.h"
#include "QRCode.h"

using namespace cv;
using namespace std;

void getBuffer(DataBuffer buffer)
{
    cout << "请输入文件名：";
    string filename;
    cin >> filename;
    if (!buffer.reload(filename))
        cout << "读取失败！" << endl;

    while (!buffer.isEnd())
    {
        char tmp = buffer.nextChar();
        cout << tmp;
    }
}

int main(int argc, char** argv)
{
    DataBuffer buffer;
    //getBuffer(buffer);

    QRCode qr;
    qr.getQRCode();
}
