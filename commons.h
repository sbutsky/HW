#pragma once

enum ErrorCodes { NOERROR = 0, UNEXPECTED_EOF = 1, INVALID_FORMAT = 2, NOTANUMBER = 3, NONRECTANGULAR_MATRIX = 4, INCORRECT_PIXEL=5, ARRAY_BOUNDS_INCORRECT=6 };

class AbstractErrCodeStorage {
public:
	virtual enum ErrorCodes err() = 0;
	virtual bool hasErr() = 0;
	virtual void clearErr() = 0;
};

class ErrCodeStorage : public AbstractErrCodeStorage {
private:
	enum ErrorCodes err_;
protected:
	void err(const enum ErrorCodes& value) { err_ = value; }
public:
	virtual enum ErrorCodes err() override { return err_; }
	virtual bool hasErr() override { return err() != ErrorCodes::NOERROR; }
	virtual void clearErr() override { err(ErrorCodes::NOERROR); }
};