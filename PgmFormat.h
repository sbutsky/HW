#pragma once
#include "commons.h"
#include "Alpha2Format.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class PgmFormat : public Alpha2Format
{
private:
	size_t nrows_;
	size_t ncols_;
	size_t graylvl_;
	bool is_alpha_format;
public:
	PgmFormat();
	virtual ~PgmFormat();
public:
	virtual bool readHeader(std::istream& src, enum Format& fmt);
	virtual bool readFormat(std::istream& src);
	virtual void writeFormat(IMatrix* m, std::ostream& out);
	virtual bool validateRowItems(const std::string& s);
public:
	template<typename T>
	bool getVector(const std::string& s, std::vector<T>& vec, int expectedNumberOfColumns) {
		std::istringstream ss1(s);
		std::string stmp;

		while (std::getline(ss1, stmp, ' ')) {
			if (stmp.length() == 0) continue;
			if (!validateRowItems(stmp)) {
				return false;
			}
			auto b = (T)std::stoi(stmp);
			if (b > graylvl_) {
				err(ErrorCodes::INCORRECT_PIXEL);
				return false;
			}
			vec.push_back(b);
		}

		return true;
	}
	size_t nrows()const { return nrows_; }
	size_t ncols()const { return ncols_; }
	size_t graylvl()const { return graylvl_; }
	bool nrows_initialized();
	bool ncols_initialized();
	bool graylvl_initialized();
};

