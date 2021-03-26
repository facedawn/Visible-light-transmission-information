#include <string>
#include <iostream>
#include "Encode.h"

using namespace std;

int main(int argc, char** argv)
{
    cout << "请输入文件名：";
    string filename;
    cin >> filename;
    cout << "请输入视频文件名（mp4格式！）：";
    string videoname;
    cin >> videoname;

    Encode encode(filename, videoname);

    bool result = false;
    if(!encode.isEmpty())
        result = encode.data2Video();

    cout << endl << "============================" << endl;
    if (result)
        cout << "成功！" << endl;
    else
        cout << "因为未知问题失败！" << endl;
    
    cout << "视频文件（demo.mp4）将会存放在\"" << videoname << "\"目录下！" << endl;
    return 0;
}
