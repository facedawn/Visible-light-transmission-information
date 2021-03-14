import datetime
import time
from pathlib import Path
import numpy as np
import cv2
from pyzbar import pyzbar
import glob


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
        binary, mat = cv2.threshold(image_input, binary, 255, cv2.THRESH_BINARY)
        res = pyzbar.decode(mat)
        binary += binary_step

    return res


if __name__ == '__main__':
    total_image = 0
    success_count = 0
    path = "VisualNet/img_temp/"

    for filename in glob.glob(path+'/*.png'):
        img = cv2.imread(filename)
        if img is None:
            print(filename + " is error!")
            continue

        start_time = time.time()
        result = get_qrcode_result(img, binary_max=230, binary_step=2)

        print(f"\n\nGot {filename} result: {result}, "
                f"using time : {datetime.timedelta(seconds=(time.time() - start_time))}")
        print('\n')
        # 换源
        str = result[0].data
        print(str.decode('utf-8'))
        print('========================')
        if len(result) > 0:
            success_count += 1
        total_image += 1

    print(f"total image = {total_image}, success count = {success_count}")
