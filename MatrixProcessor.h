#pragma once
#include <iostream>
#include <string>
#include "commons.h"
#include "BoundsMatrix.h"

class GenFormat;

class MatrixProcessor : public AbstractErrCodeStorage
{
private:
	IMatrix* matrix_src_;
	IMatrix* matrix_out_;
	GenFormat* gf_;
	size_t cols_;
	size_t rows_;
protected:
	void matrix_src(IMatrix* value) {
		release_matrix_src();
		matrix_src_ = value; 
	}
	void matrix_out(IMatrix* value) { 
		release_matrix_out();
		matrix_out_ = value; 
	}
	size_t cols()const { return cols_; }
	size_t rows()const { return rows_; }
	GenFormat* gf()const { return gf_; }
	void release_matrix_out() {
		if (matrix_out_ != NULL) {
			delete matrix_out_;
			matrix_out_ = NULL;
		}
	}
	void release_matrix_src() {
		if (matrix_src_ != NULL) {
			delete matrix_src_;
			matrix_src_ = NULL;
		}
	}
public:
	IMatrix* const matrix_src()const { return matrix_src_; }
	IMatrix* const matrix_out()const { return matrix_out_; }
	void gf(GenFormat* value) { gf_ = value; }
	virtual enum ErrorCodes err() override;
	virtual bool hasErr() override;
public:

	MatrixProcessor(GenFormat* gf):cols_(0),rows_(0),matrix_src_(NULL),matrix_out_(NULL),gf_(gf)
	{
	}

	virtual ~MatrixProcessor()
	{
		release_matrix_src();
		release_matrix_out();
	}

	virtual bool readMatrix(std::istream& src) = 0;
	virtual bool appendRow(std::string& s, int expectedNumberOfColumns=0) = 0;
	virtual void transpose() = 0;
	virtual void writeMatrix(std::ostream& out) = 0;
};

