#include "stdafx.h"
#include "commons.h"
#include "AlphaMatrixProcessor.h"
#include <sstream>
#include <string>
#include "AlphaFormat.h"

AlphaMatrixProcessor::AlphaMatrixProcessor() :MatrixProcessor(new AlphaFormat()) {
	this->gf()->processor(this);
}

bool 
AlphaMatrixProcessor::appendRow(std::string & s, int expectedNumberOfColumns)
{
	clearErr();
	if (s.length() == 0) return true;
	std::vector<char> row;
	auto af = dynamic_cast<AlphaFormat*>(gf());
	if (!af->getVector(s, row, expectedNumberOfColumns)) return false;
	
	if (matrix_src() == NULL) {
		if (row.size() == 0)
			matrix_src(new AlphaMatrix(row.size(), 0));
		else {
			matrix_src(new AlphaMatrix(row.size(), 1));
			dynamic_cast<AlphaMatrix*>(matrix_src())->assignRow(0, row);
		}
	}
	else
		dynamic_cast<BoundsMatrix<char>*>(matrix_src())->appendRow(row);
	return true;
}

void AlphaMatrixProcessor::transpose()
{
	matrix_out(dynamic_cast<AlphaMatrix*>(matrix_src())->transpose());
}

bool AlphaMatrixProcessor::readMatrix(std::istream& src) {
	enum Format fmt;
	if (!gf()->readHeader(src, fmt))
		return false;
	return gf()->readFormat(src);
}

void AlphaMatrixProcessor::writeMatrix(std::ostream & out)
{
	if(matrix_out() != NULL)
		gf()->writeFormat(matrix_out(), out);
}

void AlphaMatrixProcessor::clearErr()
{
	gf()->clearErr();
}
