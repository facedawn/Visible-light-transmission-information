#pragma once
class Points
{
	int coordinatesX;
	int coordinatesY;
public:
	Points();
	Points(int x, int y);

	void setX(int x);
	void setY(int y);
	void set(int x, int y);

	void moveLeft(int Offset);
	void moveRight(int Offset);
	void moveUp(int Offset);
	void moveDown(int Offset);
	void move(int OffsetX, int OffsetY);

	int x();
	int y();
};

