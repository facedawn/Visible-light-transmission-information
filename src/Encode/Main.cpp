#include <string>
#include <iostream>
#include "Encode.h"
#include "GlobalMacroDefine.h"

using namespace std;

void getnum(char* str, int& num); //得到数字

int main(int argc, char** argv)
{
    string filename;
    string videoname;
    int length = 0;
    if (argc == 1)
    {
        cout << "请输入文件名：";
        cin >> filename;
        cout << "请输入视频文件名（mp4格式！）：";
        cin >> videoname;
        cout << "Tips:不足一帧，取一帧，其余向下取整，当前帧率：" << RATE << "fps" << endl;
        cout << "请输入视频长度（单位：ms）：";
        cin >> length;
    }
    else if (argc == 4)
    {
        filename = argv[1];
        videoname = argv[2];
        getnum(argv[3], length);
    }

    Encode encode(filename, videoname, length, argc);

    bool result = false;
    if(!encode.isEmpty())
        result = encode.data2Video();

    cout << endl << "============================" << endl;
    if (result)
        cout << "成功！" << endl;
    else
        cout << "读取长度为0！！！" << endl;
    
    cout << "视频文件（demo.mp4）将会存放在\"" << videoname << "\"目录下！" << endl;
    return 0;
}

void getnum(char* str, int& num)
{
    num = 0;
    int i = 0;
    while (str[i])
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            num *= 10;
            num += str[i] - '0';
        }
        else if (str[i] == '.') // 忽略小数后的部分
            break;
        i++;
    }
}