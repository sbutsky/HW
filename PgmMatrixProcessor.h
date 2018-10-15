#pragma once
#include "Alpha2MatrixProcessor.h"

enum Resize { NONE, UP2, DOWN2 };

class PgmMatrixProcessor : public Alpha2MatrixProcessor
{
private:
	enum Resize rs_;
	enum Format fmt_;
public:
	PgmMatrixProcessor(enum Rotate rot, enum Resize s);
	~PgmMatrixProcessor(){}
public:
	// Inherited via MatrixProcessor
	virtual bool readMatrix(std::istream& src) override;
	virtual bool appendRow(std::string & s, int expectedNumberOfColumns = 0) override;
	virtual void transpose() override;
	virtual void writeMatrix(std::ostream & out) override;
	virtual void clearErr() override;
};

