#include "QRList.h"

QRList::QRList()
{
	QR.clear();
}

int QRList::size()
{
	return QR.size();
}
AnchorPoint QRList::indexof(int index)
{
	if (index >= size())
		return AnchorPoint();
	return QR[index];
}

void QRList::append(AnchorPoint tmp)
{
	QR.push_back(tmp);
}
bool QRList::edit(int index, AnchorPoint tmp)
{
	if (index >= size())
		return false;
	QR[index] = tmp;
	return true;
}

AnchorPoint& QRList::operator[](int i)
{
	if (i >= size()) //不够长就补长
		QR.resize(i + 1);
	return QR[i];
}