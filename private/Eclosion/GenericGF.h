#pragma once
#ifndef GENERICGF_H
#define GENERICGF_H

#include<iostream>
#include<cassert>
#include <vector>
#include <string>

class GenericGF final
{
	private:
	     std::vector<int> expTab;
	     std::vector<int> logTab;
	     int size;
	     int primitive;
	     int geneBase;

	public:
		GenericGF() { size = 0, primitive = 0,geneBase=0; }
		GenericGF(int primitive, int size, int b);
		~GenericGF()
		{
			expTab.clear();
			logTab.clear();
		};

		static int add_or_sub(int a, int b) {return a^b;}

		int exp(const int a) { return expTab[a]; }

		int log(const int a) { assert(!(a == 0)&&"Illegal Argument Exception!!!"); return logTab[a]; }

		int inverse(const int a) { assert(!(a == 0)&& "Arithmetic Exception!!"); return expTab[size - 1 - logTab[a]]; }

		int multiply(const int a, const int b){ return (a == 0 || b == 0) ? 0 : expTab[(logTab[a] + logTab[b]) % (size - 1)]; }

        int getSize(){ return size; }

		int getGeneBase(){ return geneBase; }

		std::string to_String();
};
#endif
