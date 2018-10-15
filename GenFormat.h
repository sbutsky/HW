#pragma once
#include <iostream>
#include "commons.h"

class MatrixProcessor;
class IMatrix;

enum Format { UNKNOWN, ALPHA, PGM };

class GenFormat : public ErrCodeStorage
{
private:
	MatrixProcessor* processor_;
protected:
	MatrixProcessor* processor()const { return processor_; }
public:
	static bool readLine(std::istream& src, std::string& line, enum ErrorCodes& ec, bool ignoreEOF=true);

	GenFormat():processor_(NULL)
	{
	}

	~GenFormat()
	{
	}

	void processor(MatrixProcessor* value);
public:
	virtual bool readHeader(std::istream& src, enum Format& fmt) = 0;
	virtual bool readFormat(std::istream& src) = 0;
	virtual void writeFormat(IMatrix* m, std::ostream& out) = 0;
	virtual bool validateRowItems(const std::string& s) = 0;
};

