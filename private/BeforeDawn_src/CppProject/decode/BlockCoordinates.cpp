#include "BlockCoordinates.h"

BlockCoordinates::BlockCoordinates()
{
	coordinatesLeftTop.set(0, 0);
	coordinatesRightBottom.set(0, 0);
}
BlockCoordinates::BlockCoordinates(Points p1, Points p2)
{
	coordinatesLeftTop = p1;
	coordinatesRightBottom = p2;
}
BlockCoordinates::BlockCoordinates(int x1, int y1, int x2, int y2)
{
	coordinatesLeftTop.set(x1, y1);
	coordinatesRightBottom.set(x2, y2);
}


void BlockCoordinates::reset()
{
	coordinatesLeftTop.set(0, 0);
	coordinatesRightBottom.set(0, 0);
}
Points BlockCoordinates::leftTop()
{
	return coordinatesLeftTop;
}
Points BlockCoordinates::rightBottom()
{
	return coordinatesRightBottom;
}