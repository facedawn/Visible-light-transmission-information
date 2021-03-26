#include "QRCode.h"

//=======================================
//数据定义
//=======================================
#define WihtePadding (Numbmerofpixels * 2)        //白边像素
#define MarginPadding 30        //外边距白边像素
#define BlackSquare 3           //黑框像素
#define NumberofColorBlocks 51 //矩阵大小（边长）（应满足两点：1、大小不能小于35,2、为奇数）
#define Numbmerofpixels 5      //单个点对应的像素
#define LinePixels (Numbmerofpixels * NumberofColorBlocks + 2 * WihtePadding + 2 * MarginPadding + 2 * BlackSquare)
#define DataContain (NumberofColorBlocks * NumberofColorBlocks - 192 - 30 - (NumberofColorBlocks - 16) * 2) //可以储存的数据量，217为定位点占用，30为版本信息

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
const char version[16] = {
    1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1};

//=======================================
//主体部分
//=======================================

QRCode::QRCode()
{
    init();
    buffer = NULL;
}

QRCode::QRCode(DataBuffer* _buffer)
{
    init();
    setBuffer(_buffer);
}
void QRCode::setBuffer(DataBuffer* _buffer)
{
    buffer = _buffer;
}

void QRCode::init()
{
    //初始化
    rgb_pixels.push_back({ 255, 255, 255 }); //0--白色
    rgb_pixels.push_back({ 0, 0, 0 });       //1--黑色
}
bool QRCode::isEnd()
{
    if (buffer != NULL)
        return buffer->isEnd();
    else
        return true;
}

//生成二维码
Mat QRCode::getQRCode()
{
    //初始化图片并赋值为白色
    Mat img(LinePixels, LinePixels, CV_8UC3, Scalar(255, 255, 255));

    //填写基础信息
    QRCodeBasic();
    //cout << "可以储存的二进制位数：" << DataContain << endl;

    //写入数据
    writeData();

    //异或运算
    Xor();

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
                    int x = i * Numbmerofpixels + p + WihtePadding + MarginPadding + BlackSquare;
                    int y = j * Numbmerofpixels + q + WihtePadding + MarginPadding + BlackSquare;
                    int color = (matrix[i][j] == -1) ? 0 : matrix[i][j];
                    img.at<Vec3b>(x, y) = rgb_pixels[color];
                }
            }
        }
    }
    for (int i = MarginPadding; i < MarginPadding + BlackSquare; i++)
    {
        for (int j = MarginPadding; j < LinePixels - MarginPadding; j++)
        {
            img.at<Vec3b>(i, j) = 1;
            img.at<Vec3b>(j, i) = 1;
            img.at<Vec3b>(LinePixels - i, j) = 1;
            img.at<Vec3b>(LinePixels - j, LinePixels - i) = 1;
        }
    }
    return img;
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


    //填补定位点
    for (i = 8; i < NumberofColorBlocks - 8; i++)
    {
        matrix[0][i] = (i + 1) % 2;
        matrix[i][0] = (i + 1) % 2;
    }

    //填写版本信息
    for (i = 0; i < 16; i++)
    {
        matrix[1][i + 8] = version[i];
        matrix[i + 8][1] = version[i];
    }
}

//写入数据
void QRCode::writeData()
{
    //二维码头将会占去8个字节，为了方便并未采用变长编码，后期若有时间可以优化
    const int headCover = 8;
    if (buffer == NULL)
        return;
    unsigned int size = DataContain / 8; //当前大小二维码可写字节数
    unsigned int dataSize = buffer->size();
    unsigned int pointer = buffer->pointer();
    string type = buffer->getFiletype();

    unsigned int writeSize = (dataSize - pointer > size - headCover) ? size - headCover : (dataSize - pointer);
    int x = 1; //重置写入位置指针
    int y = 1;
    unsigned short page = pointer / (size - headCover); //当前页数
    unsigned short pageTotle = dataSize / (size - headCover); //总共页数

    unsigned short pow = 1;
    for (int i = 0; i < 16; i++)
    {
        fixPoint(x, y);
        matrix[x][y] = ((page & pow) == pow) ? 1 : 0;
        pow *= 2;
    }
    pow = 1;
    for (int i = 0; i < 16; i++)
    {
        fixPoint(x, y);
        matrix[x][y] = ((pageTotle & pow) == pow) ? 1 : 0;
        pow *= 2;
    }

    unsigned int pow2 = 1;
    for (int i = 0; i < 32; i++)
    {
        fixPoint(x, y);
        matrix[x][y] = ((writeSize & pow2) == pow2) ? 1 : 0;
        pow2 *= 2;
    }

    while (writeSize-- && !buffer->isEnd())
    {
        char tmp = buffer->nextChar();
        char pow3 = 1;
        for (int i = 0; i < 8; i++)
        {
            fixPoint(x, y);
            matrix[x][y] = ((tmp & pow3) == pow3) ? 1 : 0;
            pow3 *= 2;
        }
    }
    printf("Current pages:%d\tTotal pages:%d\n", page, pageTotle);
}
void QRCode::fixPoint(int& x, int& y)
{
    //x为行，y为列
    //自动步进一步，找到可以落笔的空位。一旦溢出就会设为0,0
    x++;
    if (x >= NumberofColorBlocks)
    {
        x = 0;
        y++;
    }
    if (y >= NumberofColorBlocks)
    {
        x = NumberofColorBlocks - 1;
        y = NumberofColorBlocks - 1;
        return;
    }
    if (matrix[x][y] != -1)
        fixPoint(x, y);
}


void QRCode::Xor()
{
    for (int i = 0; i < NumberofColorBlocks; i++)
    {
        for (int j = 0; j < NumberofColorBlocks; j++)
        {
            const int block = 5; //区块大小
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

    for (int i = 8; i < NumberofColorBlocks - 8; i++)
    {
        xorMatrix[0][i] = -1;
        xorMatrix[i][0] = -1;
    }

    //填写版本信息
    for (int i = 0; i < 16; i++)
    {
        xorMatrix[1][i + 8] = -1;
        xorMatrix[i + 8][1] = -1;
    }


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
