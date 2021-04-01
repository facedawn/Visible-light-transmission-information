#include "Decode.h"


Decode::Decode()
{

}
Decode::Decode(string _output, string _filename)
{

	videoFilename = _filename;
	outputFilename = _output;
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
	buffer.savefile(outputFilename);
	return buffer.size();
}