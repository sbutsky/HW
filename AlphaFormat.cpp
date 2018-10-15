#include "stdafx.h"
#include "MatrixProcessor.h"
#include "AlphaFormat.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>


AlphaFormat::AlphaFormat():GenFormat()
{
}


AlphaFormat::~AlphaFormat()
{
}

bool 
AlphaFormat::validateRowItems(const std::string& s) {
	for (auto it = s.cbegin(); it != s.cend(); it++)
		if (!((*it >= 'a' && *it <= 'z') || (*it >= 'A' && *it <= 'Z'))) {
			err(ErrorCodes::NOTANUMBER);
			return false;
		}
	return true;
}

bool 
AlphaFormat::readHeader(std::istream& src, enum Format& fmt) {
	std::string hdr;
	enum ErrorCodes ec;
	fmt = Format::UNKNOWN;
	if (!readLine(src, hdr, ec, false)) {
		err(ec);
		return false;
	}
	if (hdr.compare("Alpha")) {
		err(ErrorCodes::INVALID_FORMAT);
		return false;
	}
	fmt = Format::ALPHA;
	return true;
}

bool 
AlphaFormat::readFormat(std::istream& src) {
	enum ErrorCodes ec;
	err(ErrorCodes::NOERROR);
	int nColumns = 0;
	std::string s;
	while (readLine(src, s, ec)) { //Ignore possible error since it is just EOF
		if (s.length() > 0) { 
			processor()->appendRow(s, nColumns);
			if (nColumns == 0) nColumns = processor()->matrix_src()->dimC();
		}
	}
	return true;
}

void
AlphaFormat::writeFormat(IMatrix* m, std::ostream& out) {
	out << "Alpha" << std::endl;
	auto bm = dynamic_cast<AlphaMatrix*>(m);
	for (size_t r = 0; r < bm->dimR(); r++) {
		auto row = bm->row(r);
		std::string s;
		for (auto cit = row.cbegin(); cit != row.cend(); cit++) {
			s.push_back(*cit);
		}
		out << s << std::endl;
	}
}

bool 
AlphaFormat::getVector(const std::string& s, std::vector<char>& vec, int expectedNumberOfColumns) {
	std::istringstream ss1(s);
	std::string stmp;

	while (std::getline(ss1, stmp, ' ')) {
		if (!AlphaFormat::validateRowItems(stmp))
			return false;
		for (auto it = stmp.begin(); it != stmp.end(); it++) {
			vec.push_back(*it);
		}
	}
	if (expectedNumberOfColumns != 0 && vec.size() != expectedNumberOfColumns) {
		err(ErrorCodes::NONRECTANGULAR_MATRIX);
		return false;
	}
	return true;
}