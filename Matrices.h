#ifndef MATRICES_HEADER
#define MATRICES_HEADER

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

template<typename T>
class Matrix1D
{
public:
	Matrix1D() = default;
	Matrix1D(int x, vector<string> xNames)
	{
		if (x != (int)xNames.size())
			throw new exception("Dimensions Do Not Match");
		this->x = x;
		for (int i = 0; i < this->x; i++)
			this->values.push_back(INFINITY);
		this->xNames = xNames;
	}
	void Assign(int x, vector<string> xNames)
	{
		if (x != (int)xNames.size())
			throw new exception("Dimensions Do Not Match");
		this->x = x;
		for (int i = 0; i < this->x; i++)
			this->values.push_back(INFINITY);
		this->xNames = xNames;
	}
	void Print()
	{
		vector<int> columnWidths;

		for (int i = 0; i < this->x; i++)
			if (this->xNames[i].length() > 5)
				columnWidths.push_back(this->xNames[i].length());
			else
				columnWidths.push_back(5);

		for (int i = 0; i < this->x; i++)
			cout << right << setw(columnWidths[i]) << this->xNames[i] << " ";
		cout << endl;

		for (int i = 0; i < this->x; i++)
			cout << right << setw(columnWidths[i]) << setprecision(3) << fixed << this->operator[](i) << " ";
		cout << endl << endl;
	}
	T& operator[](string xName)
	{
		int index;
		if ((index = GetIndexOfName(xName)) != -1)
			return this->values[index];
		else
			throw new exception("Value Name Not Found");
	}
	T& operator[](int xIndex)
	{
		if (xIndex < this->x)
			return this->values[xIndex];
		else
			throw new exception("Index Is Outside The Bounds of the Array");
	}

	int x;
	vector<string> xNames;
	vector<T> values;
	int GetIndexOfName(string xName)
	{
		for (int i = 0; i < (int)this->xNames.size(); i++)
			if (this->xNames[i] == xName)
				return i;
		return -1;
	}
};

template<typename T>
class Matrix2D
{
public:
	Matrix2D() = default;
	Matrix2D(int y, int x, vector<string> yNames, vector<string> xNames)
	{
		this->x = x; this->y = y;
		this->yNames = yNames;

		for (int i = 0; i < this->y; i++)
			this->values.push_back(Matrix1D<T>(this->x, xNames));
	}
	void Assign(int y, int x, vector<string> yNames, vector<string> xNames)
	{
		this->x = x; this->y = y;
		this->yNames = yNames;

		for (int i = 0; i < this->y; i++)
			this->values.push_back(Matrix1D<T>(this->x, xNames));
	}
	void Print()
	{
		vector<int> columnWidths;
		for (int i = 0; i < this->x; i++)
			if (this->operator[](0).xNames[i].length() > 5)
				columnWidths.push_back(this->operator[](0).xNames[i].length());
			else
				columnWidths.push_back(5);

		int rowLabelColumnWidth = 0;
		for (int i = 0; i < this->y; i++)
			if ((int)this->yNames[i].length() > rowLabelColumnWidth)
				rowLabelColumnWidth = this->yNames[i].length();

		cout << setw(rowLabelColumnWidth + 1) << " ";
		for (int i = 0; i < this->x; i++)
			cout << right << setw(columnWidths[i]) << this->operator[](0).xNames[i] << " ";
		cout << endl;

		for (int i = 0; i < this->y; i++)
		{
			cout << left << setw(rowLabelColumnWidth) << this->yNames[i] << " ";

			for (int j = 0; j < this->x; j++)
				cout << right << setw(columnWidths[j]) << setprecision(3) << fixed << this->operator[](i)[j] << " ";

			cout << endl;
		}
		cout << endl;
	}
	Matrix1D<T>& operator[](string yName)
	{
		int index;
		if ((index = GetIndexOfName(yName)) != -1)
			return this->values[index];
		else
			throw new exception("Value Name Not Found");
	}
	Matrix1D<T>& operator[](int yIndex)
	{
		if (yIndex < this->y)
			return this->values[yIndex];
		else
			throw new exception("Index Is Outside The Bounds of the Array");
	}

	int x, y;
	vector<string> yNames;
	vector<Matrix1D<T>> values;
	int GetIndexOfName(string yName)
	{
		for (int i = 0; i < (int)this->yNames.size(); i++)
			if (this->yNames[i] == yName)
				return i;
		return -1;
	}
};

template<typename T>
class Matrix3D
{
public:
	Matrix3D() = default;
	Matrix3D(int z, int y, int x, vector<string> zNames, vector<string> yNames, vector<string> xNames)
	{
		this->x = x; this->y = y; this->z = z;
		this->zNames = zNames;

		for (int i = 0; i < this->z; i++)
			this->values.push_back(Matrix2D<T>(this->y, this->x, yNames, xNames));
	}
	void Assign(int z, int y, int x, vector<string> zNames, vector<string> yNames, vector<string> xNames)
	{
		this->x = x; this->y = y; this->z = z;
		this->zNames = zNames;

		for (int i = 0; i < this->z; i++)
			this->values.push_back(Matrix2D<T>(this->y, this->x, yNames, xNames));
	}
	//Not Yet Implemented
	void Print()
	{

	}
	Matrix2D<T>& operator[](string zName)
	{
		int index;
		if ((index = GetIndexOfName(zName)) != -1)
			return this->values[index];
		else
			throw new exception("Value Name Not Found");
	}
	Matrix2D<T>& operator[](int zIndex)
	{
		if (zIndex < this->z)
			return this->values[zIndex];
		else
			throw new exception("Index Is Outside The Bounds of the Array");
	}

	int x, y, z;
	vector<string> zNames;
	vector<Matrix2D<T>> values;
	int GetIndexOfName(string zName)
	{
		for (int i = 0; i < (int)this->zNames.size(); i++)
			if (this->zNames[i] == zName)
				return i;
		return -1;
	}
};

void Log(Matrix3D<double> * input)
{
	for (int i = 0; i < input->x; i++)
		for (int j = 0; j < input->y; j++)
			for (int k = 0; k < input->z; k++)
				(*input)[k][j][i] = log((*input)[k][j][i]);
}

void Log(Matrix2D<double> * input)
{
	for (int i = 0; i < input->x; i++)
		for (int j = 0; j < input->y; j++)
			(*input)[j][i] = log((*input)[j][i]);
}

void Log(Matrix1D<double> * input)
{
	for (int i = 0; i < input->x; i++)
		(*input)[i] = log((*input)[i]);
}

void Exp(Matrix3D<double> * input)
{
	for (int i = 0; i < input->x; i++)
		for (int j = 0; j < input->y; j++)
			for (int k = 0; k < input->z; k++)
				(*input)[k][j][i] = exp((*input)[k][j][i]);
}

void Exp(Matrix2D<double> * input)
{
	for (int i = 0; i < input->x; i++)
		for (int j = 0; j < input->y; j++)
			(*input)[j][i] = exp((*input)[j][i]);
}

void Exp(Matrix1D<double> * input)
{
	for (int i = 0; i < input->x; i++)
		(*input)[i] = exp((*input)[i]);
}

#endif