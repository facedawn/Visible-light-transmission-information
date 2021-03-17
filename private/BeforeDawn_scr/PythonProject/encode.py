import datetime
import time
from pathlib import Path
import numpy as np
import cv2
from pyzbar import pyzbar
import glob
import qrcode
import re
import os.path

'''
目前暂时按照700字（汉字）来制作二维码，采用版本40，也就是最大的版本。
后期看根据情况是否需要调整

按照 10 fps 的速率来计算，传输的速度就可以达到 0.112 Mbps

所需要的库皆在上述头文件部分，在运行之前先确保安装完成所有的库

ERROR_CORRECT_L：大约7%或更少的错误能被纠正。
ERROR_CORRECT_M（默认）：大约15%或更少的错误能被纠正。
ROR_CORRECT_H：大约30%或更少的错误能被纠正。
'''

# ====================================================
# 辅助函数


def cut(obj, sec):
    return [obj[i:i+sec] for i in range(0, len(obj), sec)]


def schedule(step, index=0, page=1):
    if step == -1:
        print('0 % -- Reading file')
    elif step == 0:
        print('33 % -- String segmentation complete')
    elif step == 1:
        percent = 0.33 + ((index + 1) / page) * 0.34
        print((int(percent * 100)), '% -- Creating picture')
    else:
        print('80 % -- Composing video')

# ====================================================
# 二维码生成模块


def makeQR(__data):
    qr = qrcode.QRCode(
        version=40,
        error_correction=qrcode.constants.ERROR_CORRECT_M,
        box_size=10,
        border=1
    )  # 设置二维码的大小

    qr.add_data(__data)
    qr.make(fit=True)
    img = qr.make_image()
    return img
    # img.show()

# ====================================================
# 视频生成模块


def resize(img_array, align_mode):
    _height = len(img_array[0])
    _width = len(img_array[0][0])
    for i in range(1, len(img_array)):
        img = img_array[i]
        height = len(img)
        width = len(img[0])
        if align_mode == 'smallest':
            if height < _height:
                _height = height
            if width < _width:
                _width = width
        else:
            if height > _height:
                _height = height
            if width > _width:
                _width = width

    for i in range(0, len(img_array)):
        img1 = cv2.resize(
            img_array[i], (_width, _height), interpolation=cv2.INTER_CUBIC)
        img_array[i] = img1

    return img_array, (_width, _height)


def images_to_video(img_array, fps):
    # 图片的大小需要一致
    img_array, size = resize(img_array, 'largest')
    out = cv2.VideoWriter(
        'demo.avi', cv2.VideoWriter_fourcc(*'DIVX'), fps, size)

    for i in range(len(img_array)):
        out.write(img_array[i])
    out.release()

# ====================================================
# 数据分割、组织、展示模块


def Data2Video(file, fps):
    schedule(-1)
    data = ''
    f = open(file, 'r', encoding='utf-8')
    data = f.read()
    f.close()

    if data == '' or fps < 0:
        return False

    data_list = cut(data, 700)
    page = len(data_list)
    img_array = []

    schedule(0)

    for i in range(0, page):
        image = makeQR(str(i) + '#' + str(page) + '#' + data_list[i])
        image = cv2.cvtColor(
            255 * np.asarray(image).astype('uint8'), cv2.COLOR_RGB2BGR)
        img_array.append(image)
        schedule(1, i, page)

    schedule(2)
    images_to_video(img_array, fps)

    return True


def playVideo(fps):
    vc = cv2.VideoCapture('demo.avi')
    width = 600
    height = 600

    # 检测是否打开，打开了第一个open存储是否打开，frame取一帧的图像
    if vc.isOpened():
        isOpen, frame = vc.read()
    else:
        return

    wait_second = int(1000/fps)
    
    cv2.namedWindow("result", 0)
    cv2.resizeWindow("result", int(
        width * (height - 80) / height), height - 80)
    cv2.imshow('result', frame)
    print('Press any key to start!')
    cv2.waitKey(0)

    while isOpen:
        ret, frame = vc.read()
        if frame is None:
            break
        if ret == True:
            # gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            # cv2.imshow('result', gray)
            cv2.namedWindow("result", 0)
            cv2.resizeWindow("result", int(
                width * (height - 80) / height), height - 80)
            cv2.imshow('result', frame)
            if cv2.waitKey(wait_second) & 0xFF == 27:
                break

    vc.release()
    cv2.destroyAllWindows()

# ====================================================
# 主函数部分


def main():
    if os.path.isfile('demo.avi'):
        choice = input('The video file already exists. Do you want to continue encoding?[y|Y]:')
        if choice != 'y' and choice != 'Y':
            playVideo(10)
            return
        else:
            os.remove("demo.avi")

    noticeFileName = input('Please input the information to be sent to the original file:')
    fps = int(input('Please enter fps (positive integer):'))
    # fps = 10
    # noticeFileName = 'VisualNet/data.txt'
    if fps <= 0 or fps > 25:
        fps = 10
    res = Data2Video(noticeFileName, fps)
    if res:
        playVideo(fps)
        print('all done!')
    else:
        print('Please check if the file name is correct,'
              + 'or if there is something to send in the file,'
              + ' or if the frame rate is not a positive integer')


if __name__ == "__main__":
    main()
