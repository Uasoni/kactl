/**
 * Author: ulf lundstrom
 * Date: 2009-08-03
 * License: CC0
 * Source: my head
 * Description: 1-indexed square matrices, initially zero; vector index 0 is unused.
 * For a column state vector, set d[i][j] to the contribution of old state j
 * to new state i. Then $(M^k)v$ applies k steps; $M^0$ is the identity.
 * Arithmetic is ordinary T arithmetic, with no modulus.
 * For mod p (prime or composite, $p\ge2$), use T=ll, normalize inputs to
 * $[0,p)$, and replace BOTH multiply-adds below with
 * \texttt{acc = (acc + (\_\_int128)x * y) \% p;}.
 * Here acc is the destination and x,y are the two factors; use the same p
 * in both overloads. For $p\le10^9$, ll intermediates suffice.
 * Usage: Matrix<int, 3> matrix;
 *  matrix.d[1][1] = 1;
 *  array<int, 4> vec = {0,1,2,3};
 *  vec = (matrix^n) * vec;
 * Time: multiplication/exponentiation $O(SIZE^3)$ / $O(SIZE^3\log n)$;
 *  matrix-vector multiplication $O(SIZE^2)$.
 * Status: tested
 */
#pragma once

template<class T, int SIZE> struct Matrix {
	array<array<T, SIZE + 1>, SIZE + 1> d{};
	Matrix operator*(const Matrix& m) const {
		Matrix a;
		// Mod p: reduce after EVERY multiply-add.
		for (int i = 1; i <= SIZE; ++i) for (int j = 1; j <= SIZE; ++j)
			for (int k = 1; k <= SIZE; ++k) a.d[i][k] += d[i][j] * m.d[j][k];
		return a;
	}
	array<T, SIZE + 1> operator*(const array<T, SIZE + 1>& vec) const {
		array<T, SIZE + 1> ret{};
		// Mod p: same change here.
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
