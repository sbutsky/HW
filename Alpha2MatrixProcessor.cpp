#include "stdafx.h"
#include "commons.h"
#include "Alpha2MatrixProcessor.h"
#include <sstream>
#include <string>
#include "Alpha2Format.h"

Alpha2MatrixProcessor::Alpha2MatrixProcessor(enum Rotate rot, GenFormat* f): AlphaMatrixProcessor(f),rot_(rot)  {
	gf()->processor(this);
}

Alpha2MatrixProcessor::Alpha2MatrixProcessor(enum Rotate rot) :Alpha2MatrixProcessor(rot, new Alpha2Format()) {}

void Alpha2MatrixProcessor::transpose()
{
	switch (rot_) {
	case Rotate::DEG0:
		matrix_out(matrix_src()->clone());
		break;
	case Rotate::DEG90:
		matrix_out(matrix_src()->transpose());
		break;
	case Rotate::DEG180:
		matrix_out(matrix_src()->transpose());
		matrix_src(matrix_out()->clone());
		matrix_out(matrix_src()->transpose());
		break;
	case Rotate::DEG270:
		matrix_out(matrix_src()->transpose());
		matrix_src(matrix_out()->clone());
		matrix_out(matrix_src()->transpose());
		matrix_src(matrix_out()->clone());
		matrix_out(matrix_src()->transpose());
		break;
	default:
		break;
	}
}

void Alpha2MatrixProcessor::mirror() 
{
	matrix_out(matrix_src()->mirror_cols());
}