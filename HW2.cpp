// HW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <tuple>
#include <intrin.h>
#include "Alpha2MatrixProcessor.h"

bool validateArg(char* arg, std::tuple<enum Rotate, bool>& rotm);

int main(int argc, char** argv)
{
	//__debugbreak();
	if (argc != 2)
		return 1;
	std::tuple<enum Rotate, bool> rm;
	if (validateArg(argv[1], rm)) {
		auto mprocessor = new Alpha2MatrixProcessor(std::get<0>(rm));
		mprocessor->readMatrix(std::cin);
		mprocessor->transpose();
		if (std::get<1>(rm))
			mprocessor->mirror();
		mprocessor->writeMatrix(std::cout);
		return 0;
	}
	else {
		std::cout << "Invalid arg" << std::endl;
		return 1;
	}
}

bool validateArg(char* arg, std::tuple<enum Rotate, bool>& rotm) {
	std::map<std::string, std::tuple<enum Rotate, bool> > m = { { "0", std::make_tuple(Rotate::DEG0, false) },{ "0m", std::make_tuple(Rotate::DEG0, true) },
	                                         { "90", std::make_tuple(Rotate::DEG90, false) },{ "90m", std::make_tuple(Rotate::DEG90, true) },
											 { "180", std::make_tuple(Rotate::DEG180, false) },{ "180m", std::make_tuple(Rotate::DEG180, true) },
											 { "270", std::make_tuple(Rotate::DEG270, false) },{ "270m", std::make_tuple(Rotate::DEG270, true) } 
	                                       };
	
	std::string s(arg);
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	auto it = m.find(s);
	if (it != m.end()) {
		rotm = (*it).second;
		return true;
	}
	else {
		rotm = std::make_tuple(Rotate::DEG0, false);
		return false;
	}
}