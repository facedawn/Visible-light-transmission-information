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

QRMatrix::QRMatrix(char* ans, int width, int height)
{	
	resize(width, height);
	matrix = ans;
}
QRMatrix::~QRMatrix()
{
	if (matrix != nullptr)
		delete[] matrix;
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