#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "commons.h"
#include "GenFormat.h"

class AlphaFormat : public GenFormat
{
public:
	AlphaFormat();
	virtual ~AlphaFormat();
public:
	virtual bool readHeader(std::istream& src, enum Format& fmt);
	virtual bool readFormat(std::istream& src);
	virtual void writeFormat(IMatrix* m, std::ostream& out);
	virtual bool validateRowItems(const std::string& s);
public:
	bool getVector(const std::string& s, std::vector<char>& vec, int expectedNumberOfColumns);
};

