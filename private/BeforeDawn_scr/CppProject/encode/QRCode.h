#pragma once
class QRCode
{
	vector<Vec3b> rgb_pixels;                             //创建 rgb像素点列表
public:
	QRCode();
	void getQRCode();
	void QRCodeBasic();
	void writeData();
	void Xor();
};

