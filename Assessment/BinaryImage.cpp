#include "BinaryImage.h"

template class BinaryImage<double>;

//creates a BinaryImage matrix based on threshold -main constructor
template <class T>
BinaryImage<T>::BinaryImage(int M, int N, T* input_data, double threshold)
	:Matrix(M, N, input_data)
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int k = i * N + j;

			if (data[k] > threshold)
			{
				data[k] = 1;
			}
			else
			{
				data[k] = 0;
			}
		}
	}
}

//BinaryImage constructor overload
template <class T>
BinaryImage<T>::BinaryImage(int sizeR, int sizeC) {
	M = sizeR;
	N = sizeC;
	data = new T[M*N];
	for (int i = 0; i < M*N; i++)
	{
		data[i] = 0;
	}
}

//BinaryImage constructor overload
template <class T>
BinaryImage<T>::BinaryImage(int sizeR, int sizeC, const BinaryImage<T>& OriginalBin)
{
	M = sizeR;
	N = sizeC;

	data = new T[M*N];

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			data[i, j] = OriginalBin.get(i, j);
		}
	}
}

//BinaryImage copy constructor
template <class T>
BinaryImage<T>::BinaryImage(const BinaryImage<T>& OriginalBin)
{
	M = OriginalBin.getM();
	N = OriginalBin.getN();
	data = new T[M*N];
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int k = i * N + j;
			data[k] = OriginalBin.get(i, j);
		}
	}
}

//BinaryImage destructor
template <class T>
BinaryImage<T>::~BinaryImage()
{
	//delete data;
}