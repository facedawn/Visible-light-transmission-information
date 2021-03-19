#include <opencv2/opencv.hpp>
#include <iostream>
#include "Encode.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    cout << "请输入文件名：";
    string filename;
    cin >> filename;
    cout << "请输入视频文件名（无需带格式，自动生成avi格式）：";
    string videoname;
    cin >> videoname;

    Encode encode(filename, videoname);

    bool result = false;
    if(!encode.isEmpty())
        result = encode.data2Video();

    if (result)
        cout << "成功！" << endl;
    else
        cout << "因为未知问题失败！" << endl;
    //Encode encode;

    //QRCode qr;
    //Mat img = qr.getQRCode();    
    //imshow("img", img);
    //waitKey(0);
}
