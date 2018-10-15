#include "stdafx.h"
#include "commons.h"
#include "PgmMatrixProcessor.h"
#include <sstream>
#include <string>
#include "PgmFormat.h"

PgmMatrixProcessor::PgmMatrixProcessor(enum Rotate rot, enum Resize s) :Alpha2MatrixProcessor(rot, new PgmFormat()), rs_(s), fmt_(Format::UNKNOWN) {
	gf()->processor(this);
}

bool 
PgmMatrixProcessor::appendRow(std::string & s, int expectedNumberOfColumns)
{
	clearErr();
	if (s.length() == 0) return true;
	std::vector<size_t> row;
	auto pf = dynamic_cast<PgmFormat*>(gf());
	if (!pf->getVector<size_t>(s, row, expectedNumberOfColumns)) return false;

	if (matrix_src() == NULL) {
		matrix_src(new PgmMatrix(pf->ncols(), pf->nrows(), false));
	}
	for (auto cit = row.cbegin(); cit != row.cend(); cit++)
		dynamic_cast<PgmMatrix*>(matrix_src())->appendMatrixElement(*cit);
	return true;
}

void PgmMatrixProcessor::transpose() {
	Alpha2MatrixProcessor::transpose();
	matrix_src(matrix_out()->clone());
	switch (rs_)
	{
	case Resize::UP2:
		matrix_out(matrix_src()->expandBy2());
		matrix_src(matrix_out()->clone());
		break;
	case Resize::DOWN2:
		matrix_out(matrix_src()->shrinkBy2());
		matrix_src(matrix_out()->clone());
		break;
	default:
		break;
	}
}

bool PgmMatrixProcessor::readMatrix(std::istream& src) {
	if (!gf()->readHeader(src, fmt_))
		return false;
	if (fmt_ == Format::ALPHA)
		return dynamic_cast<PgmFormat*>(gf())->Alpha2Format::readFormat(src);
	else
		return gf()->readFormat(src);
}

void PgmMatrixProcessor::writeMatrix(std::ostream & out)
{
	if (matrix_out() != NULL) {
		if(fmt_ == Format::ALPHA)
			dynamic_cast<PgmFormat*>(gf())->Alpha2Format::writeFormat(matrix_out(), out);
		else
			gf()->writeFormat(matrix_out(), out);
	}
}

void PgmMatrixProcessor::clearErr()
{
	gf()->clearErr();
}
