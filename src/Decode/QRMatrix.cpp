#include "QRMatrix.h"

QRMatrix::QRMatrix()
{
	matrix = nullptr;
}
QRMatrix::QRMatrix(int width, int height)
{
	matrix = nullptr;
	resize(width, height);
}

void QRMatrix::resize(int width, int height)
{
	if (matrix != nullptr)
		delete[] matrix;
	w = width;
	h = height;
	matrix = new char[w * h];
}
int QRMatrix::height()
{
	return h;
}
int QRMatrix::width()
{
	return w;
}
char& QRMatrix::at(int x, int y)
{
	return *(matrix + x * w + y);
}