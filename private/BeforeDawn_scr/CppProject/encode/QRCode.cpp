#include "QRCode.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>
#include <vector>
#include "Buffer.h"

using namespace cv;
using namespace std;

//=======================================
//数据定义
//=======================================
#define WihtePadding 30        //白边像素
#define NumberofColorBlocks 29 //矩阵大小（边长）（应满足两点：1、大小不能小于21,2、为奇数）
#define Numbmerofpixels 5      //单个点对应的像素
#define LinePixels (Numbmerofpixels * NumberofColorBlocks + 2 * WihtePadding)
#define DataContain (NumberofColorBlocks * NumberofColorBlocks - 217 - 30 - (NumberofColorBlocks - 16) * 2) //可以储存的数据量，217为定位点占用，30为版本信息

char matrix[NumberofColorBlocks][NumberofColorBlocks]; //二维码信息（这是一个没有冗余的数组！）
char xorMatrix[NumberofColorBlocks][NumberofColorBlocks]; //异或遮盖层
vector<Vec3b> rgb_pixels;                             //创建 rgb像素点列表
//大定位点
const int bigAnchorPoint[8][8] = {
    1, 1, 1, 1, 1, 1, 1, 0,
    1, 0, 0, 0, 0, 0, 1, 0,
    1, 0, 1, 1, 1, 0, 1, 0,
    1, 0, 1, 1, 1, 0, 1, 0,
    1, 0, 1, 1, 1, 0, 1, 0,
    1, 0, 0, 0, 0, 0, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0 };
//小定位点
const int smallAnchorPoint[5][5] = {
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 0, 1, 0, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1 };
//版本信息（101 1010 1010 0101）
const char version[15] = {
    1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1 };

//=======================================
//主体部分
//=======================================

QRCode::QRCode()
{
    //初始化
    rgb_pixels.push_back({ 255, 255, 255 }); //0--白色
    rgb_pixels.push_back({ 0, 0, 0 });       //1--黑色
}

//生成二维码
void QRCode::getQRCode()
{
    //初始化图片并赋值为白色
    Mat img(LinePixels, LinePixels, CV_8UC3, Scalar(255, 255, 255));

    //填写基础信息
    QRCodeBasic();
    //cout << "可以储存的二进制位数：" << DataContain << endl;

    //写入数据
    //writeData();

    //异或运算
    Xor2();

    //写入图片
    for (int i = 0; i < NumberofColorBlocks; i++)
    {
        for (int j = 0; j < NumberofColorBlocks; j++)
        {
            //写入单个0、1
            for (int p = 0; p < Numbmerofpixels; p++)
            {
                for (int q = 0; q < Numbmerofpixels; q++)
                {
                    int x = i * Numbmerofpixels + p + WihtePadding;
                    int y = j * Numbmerofpixels + q + WihtePadding;
                    int color = (matrix[i][j] == -1) ? 0 : matrix[i][j];
                    img.at<Vec3b>(x, y) = rgb_pixels[color];
                }
            }
        }
    }
    imshow("img", img);
    waitKey(0);
}

//添加二维码的基础信息（定位点、矫正图、格式点）
void QRCode::QRCodeBasic()
{
    int i, j;
    //全部标上-1，代表没有数据（-1将会被当作为0输出在图片里）
    for (i = 0; i < NumberofColorBlocks; i++)
    {
        for (j = 0; j < NumberofColorBlocks; j++)
        {
            matrix[i][j] = -1;
        }
    }

    //填补大定位点
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            matrix[i][j] = bigAnchorPoint[i][j];
            matrix[NumberofColorBlocks - 1 - i][j] = bigAnchorPoint[i][j];
            matrix[i][NumberofColorBlocks - 1 - j] = bigAnchorPoint[i][j];
        }
    }
    //填补小定位点
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            matrix[NumberofColorBlocks - 5 - i][NumberofColorBlocks - 5 - j] = smallAnchorPoint[i][j];
        }
    }
    //填补定位点
    for (i = 8; i < NumberofColorBlocks - 8; i++)
    {
        matrix[6][i] = (i + 1) % 2;
        matrix[i][6] = (i + 1) % 2;
    }
    //填写版本信息
    for (i = 0; i < 8; i++)
    {
        if (i > 1)
            matrix[7 - i][8] = version[i];
        else
            matrix[8 - i][8] = version[i];
        matrix[8][NumberofColorBlocks - i - 1] = version[i];
    }
    for (i = 8; i < 14; i++)
    {
        matrix[NumberofColorBlocks - 9][i - 8] = version[i];
        matrix[8][i - 8] = version[i];
    }
    matrix[NumberofColorBlocks - 9][7] = version[14];
    matrix[8][7] = version[14];
}

//写入数据
void QRCode::writeData()
{
    for (int i = 0; i < NumberofColorBlocks; i++)
    {
        for (int j = 0; j < NumberofColorBlocks; j++)
        {
            if (matrix[i][j] == -1)
                matrix[i][j] = rand() % 2;
        }
    }
}

void QRCode::Xor1()
{
    int i, j;
    for (i = 9; i < NumberofColorBlocks - 9; i++)
    {
        for (j = 0; j < 6; j++)
        {
            int cover = (i + j + 1) % 2;
            //左
            int compare = (matrix[i][j] == -1) ? 0 : matrix[i][j];
            if (compare == cover)
                matrix[i][j] = 0;
            else
                matrix[i][j] = 1;
            //上
            compare = (matrix[j][i] == -1) ? 0 : matrix[j][i];
            if (compare == cover)
                matrix[j][i] = 0;
            else
                matrix[j][i] = 1;
        }
    }
    for (i = 9; i < NumberofColorBlocks - 9; i++)
    {
        for (j = 7; j < 9; j++)
        {
            int cover = (i + j + 1) % 2;
            //左
            int compare = (matrix[i][j] == -1) ? 0 : matrix[i][j];
            if (compare == cover)
                matrix[i][j] = 0;
            else
                matrix[i][j] = 1;
            //上
            compare = (matrix[j][i] == -1) ? 0 : matrix[j][i];
            if (compare == cover)
                matrix[j][i] = 0;
            else
                matrix[j][i] = 1;
        }
    }
    for (i = NumberofColorBlocks - 1; i > NumberofColorBlocks - 10; i--)
    {
        int cover = (i + 8 + 1) % 2;
        int compare = (matrix[i][8] == -1) ? 0 : matrix[i][8];
        if (compare == cover)
            matrix[i][8] = 0;
        else
            matrix[i][8] = 1;
    }
    for (i = 8; i < NumberofColorBlocks; i++)
    {
        for (j = 8; j < NumberofColorBlocks; j++)
        {
            if (i > NumberofColorBlocks - 10 && i < NumberofColorBlocks - 4) //去除小定位点
                if (j > NumberofColorBlocks - 10 && j < NumberofColorBlocks - 4)
                    continue;
            int cover = (i + j + 1) % 2;
            int compare = (matrix[i][j] == -1) ? 0 : matrix[i][j];
            if (compare == cover)
                matrix[i][j] = 0;
            else
                matrix[i][j] = 1;
        }
    }
}

void QRCode::Xor2()
{
    for (int i = 0; i < NumberofColorBlocks; i++)
    {
        for (int j = 0; j < NumberofColorBlocks; j++)
        {
            const int block = 5;
            int x = i / block;
            int y = j / block;
            xorMatrix[i][j] = (char)((x + y + 1) % 2);
        }
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            xorMatrix[i][j] = -1;
            xorMatrix[NumberofColorBlocks - 1 - i][j] = -1;
            xorMatrix[i][NumberofColorBlocks - 1 - j] = -1;
        }
    }
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            xorMatrix[NumberofColorBlocks - 5 - i][NumberofColorBlocks - 5 - j] = -1;
        }
    }
    for (int i = 8; i < NumberofColorBlocks - 8; i++)
    {
        xorMatrix[6][i] = -1;
        xorMatrix[i][6] = -1;
    }

    for (int i = 0; i < 8; i++)
    {
        if (i > 1)
            xorMatrix[7 - i][8] = -1;
        else
            xorMatrix[8 - i][8] = -1;
        xorMatrix[8][NumberofColorBlocks - i - 1] = -1;
    }
    for (int i = 8; i < 14; i++)
    {
        xorMatrix[NumberofColorBlocks - 9][i - 8] = -1;
        xorMatrix[8][i - 8] = -1;
    }
    xorMatrix[NumberofColorBlocks - 9][7] = -1;
    xorMatrix[8][7] = -1;

    for (int i = 0; i < NumberofColorBlocks; i++)
    {
        for (int j = 0; j < NumberofColorBlocks; j++)
        {
            if (xorMatrix[i][j] != -1)
            {
                int compare = (matrix[i][j] == -1) ? 0 : matrix[i][j];
                if (compare != xorMatrix[i][j])
                    matrix[i][j] = 1;
                else
                    matrix[i][j] = 0;
            }
        }
    }
}