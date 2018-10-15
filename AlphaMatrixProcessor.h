#pragma once
#include "MatrixProcessor.h"

class AlphaMatrixProcessor : public MatrixProcessor
{
protected:
	AlphaMatrixProcessor(GenFormat* gf):MatrixProcessor(gf) {}
public:
	AlphaMatrixProcessor();
	~AlphaMatrixProcessor(){}

	// Inherited via MatrixProcessor
	virtual bool readMatrix(std::istream& src) override;
	virtual bool appendRow(std::string & s, int expectedNumberOfColumns = 0) override;
	virtual void transpose() override;
	virtual void writeMatrix(std::ostream & out) override;
	virtual void clearErr() override;
};

