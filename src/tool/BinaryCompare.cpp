#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

int main()
{
    string file_A;
    string file_B;
    cout << "请输入文件1：";
    cin >> file_A;
    cout << "请输入文件2：";
    cin >> file_B;
    ifstream in1(file_A, ios::in | ios::binary);
    ifstream in2(file_B, ios::in | ios::binary);
    if (!in1 || !in2)
    {
        in1.close();
        in2.close();
        cout << "文件打开失败！";
        return 0;
    }
    long long wrong = 0;

    long long start = in1.tellg();
    in1.seekg(0, ios::end);
    long long end = in1.tellg();
    long long len1 = end - start;

    start = in2.tellg();
    in2.seekg(0, ios::end);
    end = in2.tellg();
    long long len2 = end - start;

    cout << endl;
    cout << endl;
    cout << "文件1位数：" << len1 * 8 << endl;
    cout << "文件2位数：" << len2 * 8 << endl;

    int firstWrongByte = 0;
    int index = 0;
    in1.seekg(0, ios::beg);
    in2.seekg(0, ios::beg);
    while (!in1.eof() && !in2.eof())
    {
        index++;
        char *tmp = new char[2];
        char *tmp2 = new char[2];
        tmp[0] = 0;
        tmp2[0] = 0;
        in1.read(tmp, 1);
        in2.read(tmp2, 1);

        int pow = 1;
        int times = 0;
        for (int i = 0; i < 8; i++)
        {
            if ((tmp[0] & pow) != (tmp2[0] & pow))
            {
                wrong++;
                int a = ((tmp[0] & pow) == pow) ? 1 : 0;
                int b = ((tmp2[0] & pow) == pow) ? 1 : 0;
                cout << "错误 " << wrong << "\t位置 " << index << "(" << times << ")\t:\t" << a << " (对比文件)\t " << b << "(原文件)\n";
            }
            pow *= 2;
            times++;
        }

        delete[] tmp;
        delete[] tmp2;

        if (wrong == 0)
            firstWrongByte++;
    }

    cout << endl;
    if (wrong > 0)
        printf("第一个错误的字节标号：%d", firstWrongByte);
    cout << endl;
    cout << "错误位数：" << wrong << endl;
    cout << "正确率：" << (double)(len2 * 8 - wrong) / (double)(len2 * 8) << endl;
    system("pause");
    return 0;
}
