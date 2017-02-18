#ifndef MATRICES_STRING_VECTOR_HEADER
#define MATRICES_STRING_VECTOR_HEADER

#include <vector>   //vector
#include <string>   //string

using namespace std;

template<typename T>
class Matrix
{
public:
	Matrix();
	Matrix(vector<string> labels);
	Matrix(vector<string> labels, vector<T> values);
	Matrix(vector<string> labels, T value);
	
	void Assign(vector<string> labels);
	void Assign(vector<string> labels, vector<T> values);
	void Assign(vector<string> labels, T value);
	
	T& operator[](string label);
	T& operator[](int index);
	Matrix<T>& operator=(const Matrix<T>& matrix);
	
private:
	vector<string> labels;
	vector<T> values;

	int GetIndexOfLabel(string label);
	void Allocate(int size);
};

typedef Matrix<double>   Matrix1D;
typedef Matrix<Matrix1D> Matrix2D;
typedef Matrix<Matrix2D> Matrix3D;

#define MATRIX1D(X_LABELS) (Matrix1D(X_LABELS))
#define MATRIX2D(X_LABELS, Y_LABELS) (Matrix2D(X_LABELS, MATRIX1D(Y_LABELS)))
#define MATRIX3D(X_LABELS, Y_LABELS, Z_LABELS) (Matrix3D(X_LABELS, MATRIX2D(Y_LABELS, Z_LABELS)))

template<typename T>
Matrix<T>::Matrix()
{

}
template<typename T>
Matrix<T>::Matrix(vector<string> labels)
{
	Assign(labels);
}
template<typename T>
Matrix<T>::Matrix(vector<string> labels, vector<T> values)
{
	Assign(labels, values);
}
template<typename T>
Matrix<T>::Matrix(vector<string> labels, T value)
{
	Assign(labels, value);
}

template<typename T>
void Matrix<T>::Assign(vector<string> labels)
{
	Allocate((int)labels.size());

	for (int i = 0; i < (int)labels.size(); i++)
		this->labels[i] = labels[i];
}
template<typename T>
void Matrix<T>::Assign(vector<string> labels, vector<T> values)
{
	Assign(labels);

	for (int i = 0; i < (int)values.size(); i++)
		this->values[i] = values[i];
}
template<typename T>
void Matrix<T>::Assign(vector<string> labels, T value)
{
	Assign(labels);

	for (int i = 0; i < (int)labels.size(); i++)
		this->values[i] = value;
}

template<typename T>
T& Matrix<T>::operator[] (string label)
{
	return operator[](GetIndexOfLabel(label));
}
template<typename T>
T& Matrix<T>::operator[] (int index)
{
	if (index >= 0 && index < (int)values.size())
		return values[index];
	else
		throw;
}
template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& matrix)
{
	Allocate((int)matrix.labels.size());

	for (int i = 0; i < (int)labels.size(); i++)
	{
		labels[i] = matrix.labels[i];
		values[i] = matrix.values[i];
	}

	return *this;
}

template<typename T>
int Matrix<T>::GetIndexOfLabel(string label)
{
	for (int i = 0; i < (int)labels.size(); i++)
		if (labels[i] == label)
			return i;

	return -1;
}

template<typename T>
void Matrix<T>::Allocate(int size)
{
	values = vector<T>(size);
	labels = vector<string>(size);
}

#endif
