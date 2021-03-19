#include "Encode.h"

Encode::Encode()
{

}
Encode::Encode(string filename, string videoFilename)
{
	buffer.reload(filename);
	video.setFilename(videoFilename);
}
Encode::Encode(DataBuffer _buffer, string videoFilename)
{
	buffer = _buffer;
	video.setFilename(videoFilename);
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
		Mat img = qr.getQRCode();    
		imshow("img", img);
		waitKey(0);
	}
}
