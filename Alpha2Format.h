#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "commons.h"
#include "AlphaFormat.h"

class Alpha2Format : public AlphaFormat
{
private:
	struct candidate_item {
		std::tuple<size_t, std::string> data;
		bool has_asterisk() { return std::get<0>(data) != std::string::npos; }
		bool is_invalid() { return std::get<0>(data) == 0; }
		std::string before_asterisk_or_full() { return std::get<1>(data); }
		std::string expand_asterisk(int expansionSize) {
			if (is_invalid())
				return std::string();
			if(!has_asterisk()) 
				return before_asterisk_or_full();
			auto bf = before_asterisk_or_full();
			auto ast_pos = std::get<0>(data);
			auto hdr = bf.substr(0, ast_pos-1);
			auto ret_exp_size = expansionSize - hdr.length();
			if (ret_exp_size <= 0)
				return hdr;
			else {
				std::string ret(ret_exp_size, bf[ast_pos - 1]);
				return hdr + ret;
			}
		}
		candidate_item(size_t ast_pos, const std::string& s):data(std::make_tuple(ast_pos, s)) {}
	};
	std::vector<candidate_item> c_items;
public:
	Alpha2Format();
	virtual ~Alpha2Format();
public:
	virtual bool readFormat(std::istream& src);
	virtual void writeFormat(IMatrix* m, std::ostream& out);
	virtual bool validateRowItems(const std::string& s);
public:
	bool getVector(const std::string& s, std::vector<char>& vec, int expectedNumberOfColumns);
};

