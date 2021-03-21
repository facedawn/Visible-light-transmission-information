#include "Points.h"

Points::Points()
{
	coordinatesX = 0;
	coordinatesY = 0;
}
Points::Points(int x, int y)
{
	coordinatesX = x;
	coordinatesY = y;
}

void Points::setX(int x)
{
	coordinatesX = x;
}
void Points::setY(int y)
{
	coordinatesY = y;
}
void Points::set(int x, int y)
{
	coordinatesX = x;
	coordinatesY = y;
}

void Points::moveLeft(int Offset)
{
	coordinatesX += Offset;
}
void Points::moveRight(int Offset)
{
	coordinatesX -= Offset;
}
void Points::moveUp(int Offset)
{
	coordinatesY += Offset;
}
void Points::moveDown(int Offset)
{
	coordinatesY -= Offset;
}
void Points::move(int OffsetX, int OffsetY)
{
	coordinatesX += OffsetX;
	coordinatesY += OffsetY;
}

int Points::x()
{
	return coordinatesX;
}
int Points::y()
{
	return coordinatesY;
}