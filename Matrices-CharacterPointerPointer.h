#ifndef MATRICES_CHARACTER_POINTER_POINTER_HEADER
#define MATRICES_CHARACTER_POINTER_POINTER_HEADER

#define _CRT_SECURE_NO_WARNINGS //Necessary for strcpy()

#include <string.h>             //strcpy(), strlen()

template<typename T>
class Matrix
{
public:
	Matrix();
	Matrix(int size, char ** labels);
	Matrix(int size, char ** labels, T * values);
	Matrix(int size, char ** labels, T value);

	~Matrix();

	void Assign(int size, char ** labels);
	void Assign(int size, char ** labels, T * values);
	void Assign(int size, char ** labels, T value);
	
	T& operator[](char * label);
	T& operator[](int index);
	Matrix<T>& operator=(const Matrix<T>& matrix);
	
private:
	int size;
	char ** labels;
	T ** values;

	int GetIndexOfLabel(char * label);
	void Allocate(int size);

	void Zero();
};

typedef Matrix<double>   Matrix1D;
typedef Matrix<Matrix1D> Matrix2D;
typedef Matrix<Matrix2D> Matrix3D;

#define MATRIX1D(X, X_LABELS) (Matrix1D(X, X_LABELS))
#define MATRIX2D(X, Y, X_LABELS, Y_LABELS) (Matrix2D(X, X_LABELS, MATRIX1D(Y, Y_LABELS)))
#define MATRIX3D(X, Y, Z, X_LABELS, Y_LABELS, Z_LABELS) (Matrix3D(X, X_LABELS, MATRIX2D(Y, Z, Y_LABELS, Z_LABELS)))

template<typename T>
Matrix<T>::Matrix()
{
	Zero();
}
template<typename T>
Matrix<T>::Matrix(int size, char ** labels)
{
	Zero();
	Assign(size, labels);
}
template<typename T>
Matrix<T>::Matrix(int size, char ** labels, T * values)
{
	Zero();
	Assign(size, labels, values);
}
template<typename T>
Matrix<T>::Matrix(int size, char ** labels, T value)
{
	Zero();
	Assign(size, labels, value);
}

template<typename T>
Matrix<T>::~Matrix()
{
	for (int i = 0; i < size; i++)
	{
		if (values[i])
			delete values[i];

		if (labels[i])
			delete[] labels[i];
	}

	if (values)
		delete[] values;

	if (labels)
		delete[] labels;

	Zero();
}

template<typename T>
void Matrix<T>::Assign(int size, char ** labels)
{
	Allocate(size);

	for (int i = 0; i < size; i++)
	{
		this->labels[i] = new char[strlen(labels[i]) + 1];
		strcpy(this->labels[i], labels[i]);

		this->values[i] = new T;
		memset(this->values[i], 0, sizeof(T));
	}
}
template<typename T>
void Matrix<T>::Assign(int size, char ** labels, T * values)
{
	Assign(size, labels);

	for (int i = 0; i < size; i++)
		*this->values[i] = *values[i];
}
template<typename T>
void Matrix<T>::Assign(int size, char ** labels, T value)
{
	Assign(size, labels);

	for (int i = 0; i < size; i++)
		*this->values[i] = value;
}

template<typename T>
T& Matrix<T>::operator[] (char * label)
{
	return operator[](GetIndexOfLabel(label));
}
template<typename T>
T& Matrix<T>::operator[] (int index)
{
	if (index >= 0 && index < size)
		return *values[index];
	else
		throw;
}
template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& matrix)
{
	this->~Matrix();

	size = matrix.size;

	labels = new char*[size];
	values = new T*[size];

	for (int i = 0; i < size; i++)
	{
		labels[i] = new char[strlen(matrix.labels[i]) + 1];
		strcpy(labels[i], matrix.labels[i]);

		values[i] = new T;
		memset(values[i], 0, sizeof(T));

		*values[i] = *matrix.values[i];
	}

	return *this;
}

template<typename T>
int Matrix<T>::GetIndexOfLabel(char * label)
{
	for (int i = 0; i < size; i++)
		if (!strcmp(this->labels[i], label))
			return i;

	return -1;
}

template<typename T>
void Matrix<T>::Allocate(int size)
{
	if (this->size != 0)
		this->~Matrix();

	values = new T*[size];
	memset(values, 0, size * sizeof(T*));

	labels = new char*[size];
	memset(labels, 0, size * sizeof(char *));
	
	this->size = size;
}

template<typename T>
void Matrix<T>::Zero()
{
	size = 0;
	values = NULL;
	labels = NULL;
}

#endif
