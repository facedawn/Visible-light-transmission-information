#pragma  once
#ifndef RSENCODER_H
#define RSENCODER_H
#include "GenericGF.h"
#include "GenericGFPoly.h"
#include <vector>
#include <algorithm>
class RSencoder final
{
	private:
		GenericGF *field;
		std::vector<GenericGFPoly*> cacheGen;
		GenericGFPoly * buildGenerator(int d);

	public:
		qrgen::RSencoder::RSencoder() {};
		RSencoder(GenericGF *field);
		void encode(std::vector<int> &E, int ecBytes);
};
#endif
