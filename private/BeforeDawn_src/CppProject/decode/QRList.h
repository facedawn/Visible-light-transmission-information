#pragma once
#include <vector>
#include "AnchorPoint.h"

using namespace std;

class QRList
{
	vector<AnchorPoint> QR;
public:
	QRList();

	int size();
	AnchorPoint indexof(int index);
	void append(AnchorPoint tmp);
	bool edit(int index, AnchorPoint tmp);

	AnchorPoint& operator[](int i);
};

