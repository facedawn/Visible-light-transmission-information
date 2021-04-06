使用压缩前include "LZ77.h"
实例化LZ77类的对象进行压缩

LZ77类提供encode公共函数进行压缩
有两个重载
int encode(string src, string dst)//src为原文件路径，dst为压缩后文件保存路径
int encode(string src, vector<byte> *dst)//src为原文件路径，dst保存压缩文件字节流，请保证dst为空，因为该函数会清空dst的原有内容
以上两个重载均返回压缩后文件的大小，以字节为单位，当压缩文件不存在时返回-1

byte类型：typedef unsigned char byte;

LZ77类提供decode公共函数进行压缩
有两个重载
int decode(string src, string dst)//src为压缩文件路径，dst为解压后文件保存路径
int decode(vector<byte> *src, string dst)//src为待解压文件字节流，dst为解压后的文件保存路径
以上两个重载均返回解压后文件的大小，以字节为单位，当压缩文件不存在时返回-1