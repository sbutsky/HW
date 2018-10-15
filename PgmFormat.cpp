#include "stdafx.h"
#include "MatrixProcessor.h"
#include "PgmFormat.h"

#include <limits>
#include <cstddef>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>


PgmFormat::PgmFormat() :nrows_(std::numeric_limits<std::size_t>::max()),
ncols_(std::numeric_limits<std::size_t>::max()),
graylvl_(std::numeric_limits<std::size_t>::max()),
is_alpha_format(false),
Alpha2Format()
{
}


PgmFormat::~PgmFormat()
{
}

bool
PgmFormat::nrows_initialized() {
	return nrows_ != std::numeric_limits<std::size_t>::max();
}

bool
PgmFormat::ncols_initialized() {
	return ncols_ != std::numeric_limits<std::size_t>::max();
}

bool
PgmFormat::graylvl_initialized() {
	return graylvl_ != std::numeric_limits<std::size_t>::max();
}

bool
PgmFormat::validateRowItems(const std::string& s) {
	if (is_alpha_format)
		return Alpha2Format::validateRowItems(s);
	for (auto it = s.cbegin(); it != s.cend(); it++)
		if (!(*it >= '0' && *it <= '9')) {
			err(ErrorCodes::NOTANUMBER);
			return false;
		}
	return true;
}

bool 
PgmFormat::readHeader(std::istream& src, enum Format& fmt) {
	std::string hdr;
	enum ErrorCodes ec;

	fmt = Format::UNKNOWN;
	while (readLine(src, hdr, ec)) {
		if (hdr.length() == 0) continue;
		if (hdr.compare("P2")) {
			if (!hdr.compare("Alpha")) {
				fmt = Format::ALPHA;
				break;
			}
			else {
				err(ErrorCodes::INVALID_FORMAT);
				return false;
			}
		}
		else {
			fmt = Format::PGM;
			break;
		}
	}
	return true;
}

bool
PgmFormat::readFormat(std::istream& src) {
	enum ErrorCodes ec;
	
	err(ErrorCodes::NOERROR);
	int nColumns = 0;
	std::string s;
	std::vector<size_t> geom;

	while (readLine(src, s, ec)) {
		if (s.length() == 0) continue;
		std::replace(s.begin(), s.end(), '\t', ' ');
		if (geom.size() < 3) {
			std::vector<size_t> vec;
			std::istringstream ss1(s);
			std::string stmp;
			while (std::getline(ss1, stmp, ' ')) {
				if (stmp.length() == 0) continue;
				if (validateRowItems(stmp)) {
					if (getVector<size_t>(stmp, vec, 0)) {
						for (auto cit = vec.cbegin(); cit != vec.end(); cit++) {
							if (geom.size() < 3) geom.push_back(*cit);
						}
					}
				}
			}
		} 
		else {
			if (!nrows_initialized() || !ncols_initialized() || !graylvl_initialized()) {
				ncols_ = geom[0];
				nrows_ = geom[1];
				graylvl_ = geom[2];
			}
			processor()->appendRow(s, nColumns);
		}
	}
	if (!processor()->matrix_src()->validate_bounds()) {
		err(ErrorCodes::ARRAY_BOUNDS_INCORRECT);
		return false;
	}
	return true;
}

void
PgmFormat::writeFormat(IMatrix* m, std::ostream& out) {
	out << "P2" << std::endl;
	auto bm = dynamic_cast<PgmMatrix*>(m);
	out << bm->dimC() << " " << bm->dimR() << std::endl;
	out << graylvl_ << std::endl;
	for (size_t r = 0; r < bm->dimR(); r++) {
		auto row = bm->row(r);
		std::string s;
		std::ostringstream ss1(s);
		int t = 0;
		for (auto cit = row.cbegin(); cit != row.cend(); cit++) {
			if (t > 0)
				ss1 << " ";
			ss1 << *cit;
			t++;
		}
		ss1.flush();
		out << ss1.str() << std::endl;
	}
}
