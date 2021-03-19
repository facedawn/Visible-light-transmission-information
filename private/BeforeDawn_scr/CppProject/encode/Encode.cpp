#include "Encode.h"
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>

#include "Buffer.h"
#include "Video.h"

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
