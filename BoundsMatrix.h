#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

class IMatrix {
public:
	virtual IMatrix* clone() = 0;
	virtual IMatrix* mirror_cols() = 0;
	virtual IMatrix* transpose() = 0;
	virtual size_t dimC() = 0;
	virtual size_t dimR() = 0;
	virtual bool validate_bounds() = 0;
	virtual IMatrix* expandBy2() = 0;
	virtual IMatrix* shrinkBy2() = 0;
};

template <typename T>
class BoundsMatrix : public IMatrix
{
	std::vector<T> inner_;
	size_t dimC_, dimR_;
private:
	BoundsMatrix(const BoundsMatrix& outer):dimC_(outer.dimC_),dimR_(outer.dimR_) {
		std::copy(outer.inner_.begin(), outer.inner_.end(), std::back_inserter(inner_));
	}
private:
	void mirror(BoundsMatrix* m) {
		for (size_t row = 0; row < m->dimR_; row++) {
			auto first = m->inner_.begin() + row * m->dimC_;
			auto last = m->inner_.begin() + (row + 1) * m->dimC_;
			std::reverse(first, last);
		}
	}
public:
	~BoundsMatrix()
	{
	}

	BoundsMatrix(size_t dimC, size_t dimR, bool doresize=true)
		: dimC_(dimC), dimR_(dimR)
	{
		if(doresize) inner_.resize(dimC_*dimR_);
	}

	virtual size_t dimC() override { return dimC_; }
	virtual size_t dimR() override { return dimR_; }

	T& operator()(unsigned int col, unsigned int row)
	{
		if (col >= dimC_ || row >= dimR_)
			throw std::out_of_range("matrix indices out of range"); // ouch
		return inner_[dimC_*row + col];
	}

	void appendRow(std::vector<T>& row) {
		dimR_++;
		inner_.resize(dimC_*dimR_);
		for (size_t j = 0; j < dimC_; j++)
			inner_[dimC_*(dimR_ - 1) + j] = row[j];
	}

	void assignRow(size_t rowNo, std::vector<T>& row) {
		for (size_t j = 0; j < dimC_; j++)
			inner_[dimC_*rowNo + j] = row[j];
	}

	void appendMatrixElement(const T& el) {
		inner_.push_back(el);
	}

	std::vector<T> row(size_t rowNo) {
		auto first = inner_.cbegin() + rowNo * dimC_;
		auto last = inner_.cbegin() + (rowNo + 1) * dimC_;
		std::vector<T> res(first, last);
		return res;
	}

	virtual IMatrix* transpose() override {
		BoundsMatrix* res = new BoundsMatrix(dimR_, dimC_);
		for (size_t row = 0; row < dimR_; row++) {
			for (size_t col = 0; col < dimC_; col++) {
				auto dstidx = col * res->dimC_ + row;
				auto srcidx = row * dimC_ + col;
				res->inner_[dstidx] = inner_[srcidx];
			}
		}
		mirror(res);
		return res;
	}

	virtual IMatrix* mirror_cols() override {
		BoundsMatrix* res = dynamic_cast<BoundsMatrix*>(clone());
		mirror(res);
		return res;
	}

	virtual IMatrix* clone() override {
		BoundsMatrix* res = new BoundsMatrix(*this);
		return res;
	}

	virtual bool validate_bounds() override {
		return inner_.size() == (dimC_ * dimR_);
	}

	virtual IMatrix* expandBy2() override {
		BoundsMatrix* res = new BoundsMatrix(0, 0, false);
		for (size_t r = 0; r < dimR_; r++) {
			auto rw = row(r);
			std::vector<T> newrw;
			for (auto cit = rw.begin(); cit != rw.end(); cit++) {
				for(auto step=0; step < 2; step++)
					newrw.push_back(*cit);
			}
			for (auto step = 0; step < 2; step++) {
				for (auto cit = newrw.begin(); cit != newrw.end(); cit++) {
					res->inner_.push_back(*cit);
				}
			}
		}
		res->dimC_ = dimC_ * 2;
		res->dimR_ = dimR_ * 2;
		return res;
	}

	virtual IMatrix* shrinkBy2() override {
		BoundsMatrix* res;
		if (dimC_ < 2 || dimR_ < 2) {
			res = new BoundsMatrix(*this);
			return res;
		}

		size_t nRows = (dimR_ % 2) == 0 ? dimR_ : dimR_ - 1;
		size_t nCols = (dimC_ % 2) == 0 ? dimC_ : dimC_ - 1;
		
		res = new BoundsMatrix(nCols / 2, 1);

		for (size_t r = 0; r < nRows; r+=2) {
			auto rw1 = row(r);
			auto rw2 = row(r+1);
			std::vector<T> newrw;
			for (size_t c = 0; c < nCols; c+=2) {
				std::vector<T> slice1(rw1.begin() + c, rw1.begin() + c + 2);
				std::vector<T> slice2(rw2.begin() + c, rw2.begin() + c + 2);
				auto avg12 = (T)((slice1[0] + slice1[1] + slice2[0] + slice2[1]) / 4);
				newrw.push_back(avg12);
			}
			if (r == 0)
				res->assignRow(r, newrw);
			else
				res->appendRow(newrw);
		}

		return res;
	}
};

typedef BoundsMatrix<char> AlphaMatrix;
typedef BoundsMatrix<size_t> PgmMatrix;

