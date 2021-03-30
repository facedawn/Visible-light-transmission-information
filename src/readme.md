# 利用可见光传输帧的软件

该目录下存放所有发布的源代码



## 提示

1. 所有代码文件的编码方式采用**UTF-8**。
2. 若是对**Python**版本有需求可以使用"*private\BeforeDawn_src\PythonProject*"下的python项目。
3. 若是**其他语言**，可以使用**动态链接库**的方式，暂未提供发布版本的动态链接库，可根据源码自行打包。
4. MakeFile未提供，请自行完成。



## encode

### 介绍

编码模块。

将二进制数据/文件转换为视频文件。



### 使用说明

Main.cpp文件并非必须，可以将自己所写的独立的Main.cpp文件替代它。

使用本项目的时候仅需要引用Encode.h即可

```c++
#include "Encode.h"
```

#### 环境配置

Visual Studio的安装配置参考：[Visual Studio 2019安装与使用](https://zhuanlan.zhihu.com/p/94998894)

环境配置参考：[VisualStudio2019安装配置OpenCV的C++环境](https://www.jianshu.com/p/5aef93bdc45c)

#### 接口说明

Encode模块提供了三个构造函数

```c++
Encode();
Encode(string filename, string videoFilename);
Encode(DataBuffer _buffer, string videoFilename);
```

第一个为默认构造方法。

第二个提供了两个参数，分别指向要传输的文件地址与要输出的视频文件地址

第三个提供了用户自定的一种方法（*主要用于传输非文件信息，传输用户自定的任何二进制信息*），第一个参数传入二进制数据缓冲类DataBuffer（*如果要使用该类需引入对应头文件*）。第二个参数为要输出的视频文件地址



Encode模块提供了两个方法

```c++
bool isEmpty();
bool data2Video();
```

第一个函数用来反馈，文件是否有打开，是否有读到数据。

第二个函数便是整个项目的核心，利用数据生成视频。

### 版本信息

详见：[版本信息](https://github.com/facedawn/Visible-light-transmission-information/blob/main/src/encodeVersion.md)



## decode

### 介绍

解码模块。

Main.cpp文件并非必须，可以将自己所写的独立的Main.cpp文件替代它。

使用本项目的时候仅需要引用Decode.h即可

```c++
#include "Decode.h"
```



### 使用说明

#### 环境配置

Visual Studio的安装配置参考：[Visual Studio 2019安装与使用](https://zhuanlan.zhihu.com/p/94998894)

环境配置参考：[VisualStudio2019安装配置OpenCV的C++环境](https://www.jianshu.com/p/5aef93bdc45c)

#### 接口说明

Decode模块提供了三个构造函数

```c++
Decode();
Decode(string _outputFilename, string _filename);
```

第一个为默认构造方法。

第二个提供了两个参数，分别指向要输出的文件地址与要解码的视频文件地址

Encode模块提供了一个方法

```c++
bool video2Data();
```

这一个函数便是整个项目的核心，利用视频解码出二进制数据。

### 版本信息

详见：[版本信息](https://github.com/facedawn/Visible-light-transmission-information/blob/main/src/decodeVersion.md)

