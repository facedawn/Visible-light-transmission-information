#include "Encode.h"

Encode::Encode()
{
	videoLenth = 0;
	playVideo = false;
}
Encode::Encode(string filename, string videoFilename, int length, int argc)
{
	buffer.reload(filename);
	video.setFilename(videoFilename);
	videoLenth = (int)(length * RATE / 1000);
	if (videoLenth == 0)
		videoLenth = 1;
	if (argc == 4)
		playVideo = false;
	else
		playVideo = true;
}
Encode::Encode(DataBuffer _buffer, string videoFilename, int length, int argc)
{
	buffer = _buffer;
	video.setFilename(videoFilename);
	videoLenth = (int)(length * RATE / 1000);
	if (videoLenth == 0)
		videoLenth = 1;
	if (argc == 4)
		playVideo = false;
	else
		playVideo = true;
}

bool Encode::isEmpty()
{
	return (buffer.size() == 0);
}

bool Encode::data2Video()
{
	QRCode qr(&buffer);
	while (!qr.isEnd())
	{
		Mat img = qr.getQRCode(videoLenth);
		video.append(img);
		if (video.size() == videoLenth)
			break;
	}
	bool result = video.saveVideo();
	if (playVideo)
		video.playVideo();
	return result;
}
