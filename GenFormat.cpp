#include <iostream>
#include <string>
#include "stdafx.h"
#include "GenFormat.h"
#include "MatrixProcessor.h"
bool
GenFormat::readLine(std::istream& src, std::string& line, enum ErrorCodes& ec, bool ignoreEOF) {
	std::string s;
	if (src.eof()) {
		if(!ignoreEOF)
			ec = ErrorCodes::UNEXPECTED_EOF;
		return false;
	}
	ec = ErrorCodes::NOERROR;
	std::getline(src, s);
	auto pos = s.find('#');
	if (pos != std::string::npos) {
		line = s.substr(0, pos);
	}
	else
		line = s;
	return true;
}

void 
GenFormat::processor(MatrixProcessor* value) {
	processor_ = value;
	processor_->gf(this);
}