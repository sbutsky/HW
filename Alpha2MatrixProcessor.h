#pragma once
#include "GenFormat.h"
#include "AlphaMatrixProcessor.h"

enum Rotate { DEG0, DEG90, DEG180, DEG270 };

class Alpha2MatrixProcessor : public AlphaMatrixProcessor
{
private:
	enum Rotate rot_;
protected:
	Alpha2MatrixProcessor(enum Rotate rot, GenFormat* f);
protected:
	enum Rotate rot() const { return rot_; }
public:
	Alpha2MatrixProcessor(enum Rotate rot);
	~Alpha2MatrixProcessor(){}

	virtual void transpose() override;
	virtual void mirror();
};

