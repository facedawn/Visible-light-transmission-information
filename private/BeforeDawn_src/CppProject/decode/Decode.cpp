#include "Decode.h"


Decode::Decode()
{

}
Decode::Decode(string _output, string _filename)
{

	videoFilename = _filename;
	outputFilename = _output;
}

bool Decode::video2Data()
{
	Video video(videoFilename);
	if (video.size() == 0)
		return false;

	printf("\n\nTotal img:%d\n", video.size());
	printf("%d\n", video.isEnd());

	while (!video.isEnd())
	{
		//Mat img = video.nextImg();
		//imshow("result", video.nextImg());
		printf("%d %d\n", video.pointer(), video.size());
		//waitKey(100);

		QRCode QR(&buffer);
		QR.decode(video.nextImg());
	}
	return true;
}