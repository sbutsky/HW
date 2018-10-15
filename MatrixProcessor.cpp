#include "stdafx.h"
#include "MatrixProcessor.h"
#include "GenFormat.h"

enum ErrorCodes 
MatrixProcessor::err() { 
	return gf_ == NULL ? ErrorCodes::NOERROR : gf_->err(); 
}

bool 
MatrixProcessor::hasErr() {
	return gf_ == NULL ? ErrorCodes::NOERROR : gf_->hasErr(); 
}
