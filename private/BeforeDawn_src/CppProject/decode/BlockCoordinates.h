#pragma once
#include "Points.h"

class BlockCoordinates
{
	Points coordinatesLeftTop;
	Points coordinatesRightBottom;
public:
	BlockCoordinates();
	BlockCoordinates(Points p1, Points p2);
	BlockCoordinates(int x1, int y1, int x2, int y2);

	void reset();
	Points leftTop();
	Points rightBottom();
};

