#include "QRList.h"
#include "Location.h"

QRList::QRList()
{
	QR.clear();
	string start = "C:\\Users\\xc\\Desktop\\课\\计网\\项目\\data\\pic\\x", end = ".png";
	string start_2 = "C:\\Users\\xc\\Desktop\\课\\计网\\项目\\data\\res\\x";
	int i = 0;
	while (++i)
	{
		//if (i > 1)return 0;
		printf("\n\n%d:\n\n", i);

		string num = int2str(i);
		string path = start + num + end;
		src = imread(path, 1);
		//src = imread("out.jpg");
		//canny(src);
		if (src.empty()) {
			//检查是否读取图像
			cout << "Error! Input image cannot be read...\n";
			return;
		}
		Location a = Location(src);
		append(a.res);
	}
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