#pragma once
#ifndef GENERICGFPOLY_H
#define GENERICGFPOLY_H
#include "GenericGF.h"
#include<vector>
#include<string>
class  GenericGFPoly final
{
	public:
		GenericGF *field;
		std::vector<int> coeff;

	public:
		GenericGFPoly(GenericGF *field, std::vector<int> &coef);

		~GenericGFPoly() { coeff.clear(); coeff.swap(std::vector<int>()); }

		GenericGFPoly * getZero();

		GenericGFPoly * getOne();

		std::vector<int> getCoeff() { return coeff; }

		int getDegree() { return static_cast<int>(coeff.size())-1; }

		bool isZero() { return coeff[0] == 0; }

		int getCoeff(const int degree) { return coeff[coeff.size() - 1 - degree]; }

		int evaluateAt(const int a);

		GenericGFPoly* add_or_sub(GenericGFPoly* other);

		GenericGFPoly* multiply(const int scalar);

		GenericGFPoly* multiply(GenericGFPoly *other);

		GenericGFPoly* multiply_by_monomial(const int d, const int c);

		std::vector<GenericGFPoly*> divide(GenericGFPoly *other);

		GenericGFPoly * buildMonomial(int d, int c);
		std::string to_string();
};
#endif

