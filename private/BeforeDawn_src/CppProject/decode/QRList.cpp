#include "QRList.h"

QRList::QRList()
{
	QR.clear();
}

int QRList::size()
{
	return QR.size();
}
QRMatrix& QRList::at(int index)
{
	return QR[index];
}

void QRList::append(QRMatrix tmp)
{
	QR.push_back(tmp);
}
bool QRList::edit(int index, QRMatrix tmp)
{
	if (index >= size())
		return false;
	QR[index] = tmp;
	return true;
}

QRMatrix& QRList::operator[](int i)
{
	if (i >= size()) //不够长就补长
		QR.resize(i + 1);
	return QR[i];
}