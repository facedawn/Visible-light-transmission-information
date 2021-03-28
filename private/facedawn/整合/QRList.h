#pragma once
#include <vector>
#include "QRMatrix.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class QRList
{
	vector<QRMatrix> QR;
public:
	QRList();

	int size();
	QRMatrix& at(int index);
	void append(QRMatrix tmp);
	bool edit(int index, QRMatrix tmp);

	QRMatrix& operator[](int i);
};

