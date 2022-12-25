#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;

class Matrix {
private:
	int num_rows = 0;
	int num_cols = 0;

	vector<vector<int>> elems;
public:
	Matrix() {
	}
	Matrix(int a, int b) {
		Reset(a, b);
	}
	void Reset(int a, int b) {
		if (a < 0 || b < 0)
			throw out_of_range("Num of rows and num of coloms must be >= 0");
		if (a == 0 || b == 0) {
			a = b = 0;
		}
		num_rows = a;
		num_cols = b;
		elems.assign(num_rows, vector<int>(num_cols, 0));
	}
	int At(int a, int b) const {
		return elems.at(a).at(b);
	}
	int& At(int a, int b) {
		return elems.at(a).at(b);
	}
	int GetNumRows() const {
		return num_rows;
	}
	int GetNumColumns() const {
		return num_cols;
	}
};

istream& operator>>(istream& stream, Matrix& matrix) {
	int r, c;
	stream >> r >> c;
	matrix.Reset(r, c);
	for (auto i = 0; i < r; ++i) {
		for (auto j = 0; j < c; ++j) {
			stream >> matrix.At(i, j);
		}
	}
	return stream;
}

ostream& operator<<(ostream& stream, const Matrix& matrix) {
	auto r = matrix.GetNumRows();
	auto c = matrix.GetNumColumns();

	stream << r << " " << c << endl;
	for (auto i = 0; i < r; ++i) {
		for (auto j = 0; j < c; ++j) {
			stream << matrix.At(i, j) << " ";
		}
		stream << endl;
	}
	return stream;
}

bool operator==(const Matrix& lhs, const Matrix& rhs) {
	if (lhs.GetNumRows() != rhs.GetNumRows())
		return false;
	if (lhs.GetNumColumns() != rhs.GetNumColumns())
		return false;
	for (auto r = lhs.GetNumRows(), i = 0; i < r; ++i)
		for (auto c = lhs.GetNumColumns(), j = 0; j < c; ++j)
			if (lhs.At(i, j) != rhs.At(i, j))
				return false;
	return true;
}
Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
	if (lhs.GetNumRows() != rhs.GetNumRows() )
		throw invalid_argument("invalid_argument");
	if (lhs.GetNumColumns() != rhs.GetNumColumns())
		throw invalid_argument("invalid_argument");

	auto r = lhs.GetNumRows();
	auto c = lhs.GetNumColumns();
	Matrix M(r, c);
	for (auto i = 0; i < r; ++i) {
		for (auto j = 0; j < c; ++j) {
			M.At(i, j) = lhs.At(i, j) + rhs.At(i, j);
		}
	}
	return M;
}

int main() {


	Matrix one(5, 5);
	Matrix two;

	cin >> one >> two;
	cout << one + two << endl;
	return 0;
}