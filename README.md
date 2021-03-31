# 请勿把Beta分支合并到main！！



# Visible-light-transmission-information

这里是测试版本，实验新功能加强稳定性。



## 基本介绍

计算机网络课程项目库（未完成）
内部应该含有两个程序：1.根据用户给出的信息以及自定义编码生成视频；2.根据含有生成视频的视频，读取出其蕴含的信息


编码方式：暂定为类似于二维码的编码方式

左上、右上、左下各有三个大的定位点，用于定位图片位置。右下角一个小的定位点，用于确定图片的旋转角度

其余格子中，黑格为1，白格为0，是真正需要传输的数据。

应该还需要设定一种检验码，用于判定传输数据是否有效。（暂未想好）

使用场景：

用ffmpeg将生成的图像编码成视频1。

使用摄影工具获得包含视频1的视频2.

再通过视频2获得视频1中包含的信息。

## 功能说明

该项目可以将任意类型的二进制文件编码为视频文件，通过摄像头传输到另外一台电脑上。

具体使用说明详见：[使用说明](https://github.com/facedawn/Visible-light-transmission-information/blob/main/src/readme.md)



## 文件结构

### Pirvate

存放各组员独立完成的部分

### release

最新发布的版本

### src

项目完整源代码



## 版本信息

详见：[版本信息](https://github.com/facedawn/Visible-light-transmission-information/blob/main/src/readme.md)



## 授权信息

详见：[授权信息](https://github.com/facedawn/Visible-light-transmission-information/blob/main/LICENSE)