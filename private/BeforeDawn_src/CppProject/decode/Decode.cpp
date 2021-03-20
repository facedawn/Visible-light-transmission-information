#include "Decode.h"


Decode::Decode()
{

}
Decode::Decode(string _output, string _filename)
{

	videoFilename = _output;
	outputFilename = _filename;
}

bool Decode::video2Data()
{
	Video video;
	if (!video.loadfile(videoFilename))
		return false;
	printf("\n\nTotal img:%d\n", video.size());
	printf("%d", video.isEnd());


	while (!video.isEnd())
	{
		Mat img = video.nextImg();
		imshow("result", img);
		waitKey(100);
	}
	return true;
}