#include <iostream>
#include <fstream>
#include <istream>
using namespace std;

#include "Matrix.h"

template class Matrix<double>;

//Matrix constructor
template <class T>
Matrix<T>::Matrix(int sizeR, int sizeC, T* input_data){
	M = sizeR;
	N = sizeC;
	data = new T[M*N];
	for (int i = 0; i < M*N; i++)
	{
		data[i] = input_data[i];
	}

	//std::cout << "Matrix::Matrix(int sizeR, int sizeC, double* input_data) has been invoked" << endl;
}

//Matrix constructor overload
template <class T>
Matrix<T>::Matrix(int sizeR, int sizeC){
	M = sizeR;
	N = sizeC;
	data = new T[M*N];
	for (int i = 0; i < M*N; i++)
	{
		data[i] = 0;
	}
}

//Matrix constructor overload
template <class T>
Matrix<T>::Matrix(int sizeR, int sizeC, const Matrix<T>& OriginalMatrix)
{
	M = sizeR;
	N = sizeC;

	data = new T[M*N];

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			data[i, j] = OriginalMatrix.get(i, j);
		}
	}
}

//get a value within a Matrix
template <class T>
T Matrix<T>::get(int i, int j)const
{
	int a = i*N + j;
	return data[a];
}

//set a value within a Matrix
template <class T>
void Matrix<T>::set(int i, int j, T val)
{
	data[i*N + j] = val;
}

//get a submatrix from a Matrix
template <class T>
Matrix<T> Matrix<T>::getBlock(int row, int column, int endR, int endC){
	int noRows = endR - row;
	int noColumns = endC - column;
	T* block_data = new T[noRows * noColumns];

	//gets data from a Matrix, stores data in block to return in a new Matrix
	for (int i = 0; i < noRows; i++)
	{
		for (int j = 0; j < noColumns; j++)
		{
			int k = (row + i) * N + (column + j);
			block_data[i * noColumns + j] = data[k];
		}
	}
	Matrix<T> m(noRows, noColumns, block_data);
	/*delete data after stored in Matrix to free memory.
	getBlock function is called often so if it is not freed
	it will turn into large memory leaks*/
	delete[] block_data;
	//return the Matrix
	return m;
}

//calculate sum of Matrix
template <class T>
double Matrix<T>::sum()
{
	double total = 0.0;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			total = total + data[i*N + j];
		}
	}
	return total;
}

//return height of Matrix
template <class T>
int Matrix<T>::getM() const
{
	return M;
}

//return width of Matrix
template <class T>
int Matrix<T>::getN() const
{
	return N;
}

//assignment operator for '-'
template <class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& OriginalMatrix)
{
	Matrix C = Matrix(M, N);
	double temp;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			temp = data[i*N + j] - OriginalMatrix.get(i, j);
			C.set(i, j, temp);
		}
	}

	return C;
}

//assignment operator for '*'
template <class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& OriginalMatrix)
{
	Matrix C = Matrix(M, N);
	double temp;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int k = i*N + j;
			temp = data[k] * OriginalMatrix.get(i, j);
			C.set(i, j, temp);
		}
	}

	return C;
}

template <class T>
double Matrix<T>::operator() (int i, int j)
{
	return data[i*N + j];
}

//assignment operator for '='
template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& OriginalMatrix)
{
		M = OriginalMatrix.getM();
		N = OriginalMatrix.getN();
		delete[] data;
		data = new T[M*N];
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				data[i*N + j] = OriginalMatrix.get(i, j);
			}
		}
	return *this;
}

//copy constructor
template <class T>
Matrix<T>::Matrix(const Matrix<T>& OriginalMatrix){
	N = OriginalMatrix.getM();
	M = OriginalMatrix.getN();

	data = new T[M*N];

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int k = i*N + j;
			data[k] = OriginalMatrix.get(i, j);
		}
	}

	//cout << "copy constructor invoked";
}

/*Matrix destructor, deletes data of a Matrix when it goes out of scope
freeing memory used by the data*/
template <class T>
Matrix<T>::~Matrix(){
	delete[] data;
}


//write Matrix of data to a PGM file type
template <class T>
void Matrix<T>::writePGM(char* fileName, int Q)
{
	int x = getM();
	int y = getN();
	char *image;
	ofstream myfile;

	image = new char[x*y];

	// convert the integer values to unsigned char

	for (int i = 0; i<x; i++)
	{
		for (int j = 0; j<y; j++)
		{
			image[i*y + j] = data[i*N + j];
		}
	}

	myfile.open(fileName, ios::out | ios::binary | ios::trunc);

	if (!myfile) {
		std::cout << "Can't open file: " << fileName << endl;
		exit(1);
	}

	myfile << "P5" << endl;
	myfile << y << " " << x << endl;
	myfile << Q << endl;

	myfile.write(image, (x*y) * sizeof(char));

	if (myfile.fail()) {
		std::cout << "Can't write image " << fileName << endl;
		exit(0);
	}

	myfile.close();

	delete[] image;
}