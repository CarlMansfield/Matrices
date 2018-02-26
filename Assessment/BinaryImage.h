#ifndef BINARYIMAGE_H
#define BINARYIMAGE_H

#include "Matrix.h"

#pragma once
template <class T>
class BinaryImage : public Matrix<T>
{
public:
	BinaryImage(int sizeR, int sizeC, T* input_data, double threshold);
	BinaryImage(int sizeR, int sizeC);
	BinaryImage(int sizeR, int sizeC, const BinaryImage<T>& OriginalBin);
	~BinaryImage();
	BinaryImage(const BinaryImage<T>& OriginalBin);
};

#endif
