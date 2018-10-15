// HW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <tuple>
#include <map>
#include <limits>
#include <cstdlib>
#include <memory>
#include <intrin.h>
#include "PgmMatrixProcessor.h"

bool validateArgs(int argc, char** argv, std::tuple<enum Rotate, enum Resize, bool>& rsm, std::vector<std::string>& files);
void process(const std::tuple<enum Rotate, enum Resize, bool>& rsm, std::istream& istrm);

int main(int argc, char** argv)
{
	__debugbreak();
	std::tuple<enum Rotate, enum Resize, bool> rsm;
	std::vector<std::string> filelist;
	if (validateArgs(argc, argv, rsm, filelist)) {
		if (filelist.size() == 0) {
			process(rsm, std::cin);
		}
		else {
			for (auto filename = filelist.cbegin(); filename != filelist.cend(); filename++) {
				std::ifstream in((*filename).c_str(), std::ifstream::in);
				process(rsm, in);
			}
		}
		return 0;
	}
	else
		return 1;
}

void process(const std::tuple<enum Rotate, enum Resize, bool>& rsm, std::istream& istrm) {
	std::auto_ptr<PgmMatrixProcessor> mprocessor(new PgmMatrixProcessor(std::get<0>(rsm), std::get<1>(rsm)));
	if (mprocessor->readMatrix(istrm)) {
		mprocessor->transpose();
		if (std::get<2>(rsm))
			mprocessor->mirror();
		mprocessor->writeMatrix(std::cout);
	}
}

bool check_if_any_opt_specified(const std::map<char, bool>& m) {
	for (auto cit = m.cbegin(); cit != m.cend(); cit++) {
		if ((*cit).second)
			return true;
	}
	return false;
}

bool validateArgs(int argc, char** argv, std::tuple<enum Rotate, enum Resize, bool>& rsm, std::vector<std::string>& files) {
	std::vector<std::string> args;
	enum Rotate rot = Rotate::DEG0;
	enum Resize rs = Resize::NONE;
	bool mirr = false;
	bool fill_filelist = false;
	std::map<char, bool> option_processed = { {'h', false}, {'m', false}, {'d', false}, {'r', false} };
	rsm = std::make_tuple(rot, rs, mirr);
	for (auto a = 1; a < argc; ) {
		std::string s(argv[a]);
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		if (s[0] != '-') {
			if (!check_if_any_opt_specified(option_processed)) {
				std::cout << "No option specified" << std::endl;
				return false;
			}
			else {
				fill_filelist = true;
				files.push_back(s);
				a++;
				continue;
			}
		}
		if (fill_filelist) {
			files.push_back(s);
			a++;
			continue;
		}
		std::string sopt = s.substr(1);
		for (auto cit = sopt.cbegin(); cit != sopt.cend(); cit++) {
			char copt = *cit;
			auto optkey = option_processed.find(copt);
			if (optkey == option_processed.cend()) {
				std::cout << "Unknown option -" << copt << std::endl;
				return false;
			}
			else
				if (option_processed[copt]) { 
					a++;
					continue; 
				}
			if (copt == 'd')
				rs = Resize::UP2;
			else
				if (copt == 'h')
					rs = Resize::DOWN2;
				else if (copt == 'm')
					mirr = true;
				else if (copt == 'r') {
					if (a == argc - 1) {
						std::cout << "Invalid rotate arg" << std::endl;
						return false;
					}
					else {
						std::string v(argv[a + 1]);
						int deg;
						bool ok;
						try {
							deg = std::stoi(v);
							ok = deg >= 0 && (deg % 90) == 0;
						}
						catch (const std::out_of_range & e) {
							ok = false;
						}
						catch (const std::invalid_argument & e) {
							ok = false;
						}
						if (!ok) {
							std::cout << "Invalid rotate arg" << std::endl;
							return false;
						}
						if (deg == 0 || deg % 360 == 0) rot = Rotate::DEG0;
						else
							if (deg > 360) {
								deg = deg % 360;
							}
						std::map<int, enum Rotate> m = {
							{ 90, Rotate::DEG90 },
							{ 180, Rotate::DEG180 },
							{ 270, Rotate::DEG270 }
						};
						rot = m[deg];
						a++;
					}
				}
				option_processed[copt] = true;
		}
		a++;
	}
	rsm = std::make_tuple(rot, rs, mirr);
	return true;
}
