import qrcode

# 二维码生成模块


def makeQR(__data, __fileName):
    qr = qrcode.QRCode(
        version=40,
        error_correction=qrcode.constants.ERROR_CORRECT_L,
        box_size=10,
        border=1
    )  # 设置二维码的大小

    qr.add_data(__data)
    qr.make(fit=True)
    img = qr.make_image()
    img.save(__fileName)
    # img.show()


index = 9
data = '第' + str(index) + '个二维码'
fileName = 'VisualNet/img_temp/' + str(index) + '.png'
makeQR(data, fileName)
print('done!')
