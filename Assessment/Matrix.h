#ifndef MATRIX_H
#define MATRIX_H
#pragma once

template <class T>
class Matrix
{
public:
	Matrix(int sizeR, int sizeC, T* input_data);
	Matrix(int sizeR, int sizeC);
	Matrix(int sizeR, int sizeC, const Matrix<T>& OriginalMatrix);
	T get(int i, int j)const;
	void set(int i, int j, T val);
	int getN() const;
	int getM() const;
	double sum();
	Matrix<T> operator -(const Matrix<T>& OriginalMatrix);
	Matrix<T> operator *(const Matrix<T>& OriginalMatrix);
	Matrix<T>& operator =(const Matrix<T>& OriginalMatrix);
	Matrix<T> getBlock(int row, int column, int endR, int endC);
	double operator () (int i, int j);
	~Matrix();
	Matrix(const Matrix<T>& OriginalMatrix);
	void writePGM(char* fileName, int Q);
	T* data;
protected:
	int M;
	int N;
	Matrix(){ M = 0; N = 0; data = 0; }
};

#endif // !MATRIX_H