#include "stdafx.h"
#include "Alpha2MatrixProcessor.h"
#include "Alpha2Format.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>


Alpha2Format::Alpha2Format():AlphaFormat(),c_items()
{
}


Alpha2Format::~Alpha2Format()
{
}

bool 
Alpha2Format::validateRowItems(const std::string& s) {
	bool r = AlphaFormat::validateRowItems(s);
	if (!r)
		return false;
	auto astrksPos = s.find('*');
	if (astrksPos != std::string::npos) {
		auto astrskLastPos = s.find_last_of('*');
		if (astrksPos != astrskLastPos)
			return false; //Only one asterisk per line is allowed
	}
	return true;
}

bool 
Alpha2Format::readFormat(std::istream& src) {
	enum ErrorCodes ec;
	
	err(ErrorCodes::NOERROR);
	
	int nColumns = 0;
	c_items.clear();
	size_t rowLength = 0;
	std::string s;

	while (readLine(src, s, ec)) { //Ignore possible error since it is just EOF
		if (s.length() > 0) {
			s.erase(std::remove_if(s.begin(), s.end(), std::isspace), s.end());
			if (s.length() > 0) {
				auto ast_pos = s.find('*');
				c_items.push_back(candidate_item(ast_pos, s));
				auto l = s.length();
				if (ast_pos == std::string::npos && l > rowLength) {
					rowLength = l;
				}
			}
		}
	}

	for (auto it = c_items.begin(); it != c_items.end(); it++) {
		auto s = (*it).expand_asterisk(rowLength);
		dynamic_cast<Alpha2MatrixProcessor*>(processor())->Alpha2MatrixProcessor::appendRow(s, nColumns); //PgmMatrixProcessor has a different functionality
		if (nColumns == 0) nColumns = processor()->matrix_src()->dimC();
	}
	return true;
}

void
Alpha2Format::writeFormat(IMatrix* m, std::ostream& out) {
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
Alpha2Format::getVector(const std::string& s, std::vector<char>& vec, int expectedNumberOfColumns) {
	std::istringstream ss1(s);
	std::string stmp;

	while (std::getline(ss1, stmp, ' ')) {
		if (!validateRowItems(stmp))
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