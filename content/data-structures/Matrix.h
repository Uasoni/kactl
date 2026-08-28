/**
 * Author: ulf lundstrom
 * Date: 2009-08-03
 * License: CC0
 * Source: my head
 * Description: basic operations on 1-indexed square matrices.
 * Usage: Matrix<int, 3> matrix;
 *  matrix.d[1][1] = 1;
 *  array<int, 4> vec = {0,1,2,3};
 *  vec = (matrix^n) * vec;
 * Status: tested
 */
#pragma once

template<class T, int SIZE> struct Matrix {
	array<array<T, SIZE + 1>, SIZE + 1> d{};
	Matrix operator*(const Matrix& m) const {
		Matrix a;
		for (int i = 1; i <= SIZE; ++i) for (int j = 1; j <= SIZE; ++j)
			for (int k = 1; k <= SIZE; ++k) a.d[i][k] += d[i][j] * m.d[j][k];
		return a;
	}
	array<T, SIZE + 1> operator*(const array<T, SIZE + 1>& vec) const {
		array<T, SIZE + 1> ret{};
		for (int i = 1; i <= SIZE; ++i) for (int j = 1; j <= SIZE; ++j)
			ret[i] += d[i][j] * vec[j];
		return ret;
	}
	Matrix operator^(ll p) const {
		assert(p >= 0);
		Matrix a, b(*this);
		for (int i = 1; i <= SIZE; ++i) a.d[i][i] = 1;
		while (p) {
			if (p&1) a = a*b;
			b = b*b;
			p >>= 1;
		}
		return a;
	}
};
