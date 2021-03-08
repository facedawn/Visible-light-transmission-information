from pathlib import Path
import numpy as np
import cv2
from pyzbar import pyzbar
import glob
import qrcode
import re
import os.path

'''
这里是解码部分，基本原理是将视频读取进来，然后一帧一帧地尝试能否获取到二维码中所含有的信息。

因为每个二维码的头文件部分都是有一个数字编号，所以能够对所获取的信息进行组织总结。

后期设想在读取同一个二维码多次的时候，进行相互之间的纠正。先不考虑该情况，仅仅只考虑第一次获取的信息。
'''

# ====================================================
# 解码部分


def get_qrcode_result(image_input, binary_max=230, binary_step=2):
    # 获取二维码的结果
    #:param image_input: 输入图片数据
    #:param binary_max: 二值化的最大值
    #:param binary_step: 每次递增的二值化步长
    #:return: pyzbar 预测的结果

    # 把输入图像灰度化
    if len(image_input.shape) >= 3:
        image_input = cv2.cvtColor(image_input, cv2.COLOR_RGB2GRAY)

    # 获取自适配阈值
    binary, _ = cv2.threshold(image_input, 0, 255, cv2.THRESH_OTSU)

    # 二值化递增检测
    res = []
    while (binary < binary_max) and (len(res) == 0):
        binary, mat = cv2.threshold(
            image_input, binary, 255, cv2.THRESH_BINARY)
        res = pyzbar.decode(mat)
        binary += binary_step

    return res


def decode(file_path, output_path):
    vc = cv2.VideoCapture(file_path)

    # 检测是否打开，打开了第一个open存储是否打开，frame取一帧的图像
    if vc.isOpened():
        isOpen, frame = vc.read()
    else:
        isOpen = False

    now_index = -1
    max_index = 0
    ans = ''
    ret = True

    while isOpen:
        if frame is None:
            break
        if ret == True:
            result = get_qrcode_result(frame, binary_max=230, binary_step=2)
            str = result[0].data.decode('utf-8')
            # print(str)

            if now_index == -1:
                # 获取头信息，顺便进行版本检测，以防因为版本问题导致读取错误。
                str = str.split('#')
                if len(str) >= 2:
                    if int(str[0]) > 0 and str[1] == 'made by Moyulingjiu, 2021':
                        max_index = int(str[0])
                        now_index = 0
                elif int(str[0]) > 0:
                    max_index = int(str[0])
                    now_index = 0
            else:
                if now_index >= max_index:
                    break
                else:
                    str_temp = str.split('#')
                    if len(str_temp) >= 2:
                        # 这里要用大于等于，因为中间的一页可能漏掉了，因为各种干扰导致未能识别。
                        if int(str_temp[0]) >= now_index:
                            ans += str[len(str_temp[0]) + 1:]
                            now_index = int(str_temp[0]) + 1
        ret, frame = vc.read()
    f = open(output_path, 'w', encoding='utf-8')
    f.write(ans)
    f.close()
    return True

# ====================================================
# 主函数部分


def main():
    file_path = input('Please enter the address of the video file:')
    if not os.path.isfile(file_path):
        print('File does not exist!')
        return
    output_path = input(
        'Please input the file address of the output information:')
    if decode(file_path, output_path):
        print('All done!')
    else:
        print('Unexpected error!')


if __name__ == '__main__':
    main()
