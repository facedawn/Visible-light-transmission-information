#pragma once

class QRMatrix
{
	char* matrix;
	int w;
	int h;
public:
	QRMatrix();
	QRMatrix(int width, int height);
	QRMatrix(char* data, int width, int height);

	void resize(int width, int height);
	int height();
	int width();
	char& at(int x, int y);
};

