// HW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <intrin.h>
#include "AlphaMatrixProcessor.h"

int main()
{
	//__debugbreak();
	auto mprocessor = new AlphaMatrixProcessor();
	mprocessor->readMatrix(std::cin);
	mprocessor->transpose();
	mprocessor->writeMatrix(std::cout);
    return 0;
}

