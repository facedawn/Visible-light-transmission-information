#include "Decode.h"


Decode::Decode()
{

}
Decode::Decode(string _videoFilename, string _outputFilename, string _correctFilename)
{

	videoFilename = _videoFilename;
	outputFilename = _outputFilename;
	correctFilename = _correctFilename;
	video.loadfile(videoFilename);
}

int Decode::video2Data()
{
	if (video.size() == 0)
		return -1;

#ifdef DEBUG
	printf("\n\nTotal img:%d\n", video.size());
	printf("%d\n", video.isEnd());
#endif // !DEBUG

	while (!video.isEnd())
	{
		//Mat img = video.nextImg();
		//imshow("result", video.nextImg());
#ifdef DEBUG
		printf("\nnow:%d\ttotal:%d\n", video.pointer(), video.size());
#endif
		//waitKey(100);

		QRCode QR(&buffer);
		QR.decode(video.nextImg());
	}
	buffer.savefile(outputFilename, correctFilename);
	return buffer.size();
}