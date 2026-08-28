/**
 * Author: ulf lundstrom, simon lindholm
 * Date: 2009-08-15
 * License: CC0
 * Source: https://en.wikipedia.org/wiki/tridiagonal_matrix_algorithm
 * Description: $x=\textrm{tridiagonal}(d,p,q,b)$ solves a 1-indexed equation
 * system; all vectors have index 0 unused.
Here $diag[i]$ is the diagonal, $super[i]$ is the coefficient at
$(i,i+1)$, and $sub[i]$ is the coefficient at $(i+1,i)$.
fails if the solution is not unique.

if $|d_i| > |p_i| + |q_{i-1}|$ for all $i$, or $|d_i| > |p_{i-1}| + |q_i|$, or the matrix is positive definite,
the algorithm is numerically stable and neither \texttt{tr} nor the check for \texttt{diag[i] == 0} is needed.
 * Time: O(N)
 * Status: brute-force tested mod 5 and 7 and stress-tested for real matrices obeying the criteria above.
 */
#pragma once

typedef double T;
vector<T> tridiagonal(vector<T> diag, const vector<T>& super,
		const vector<T>& sub, vector<T> b) {
	int n = (int)b.size() - 1; vector<int> tr(n + 1);
	for (int i = 1; i < n; ++i) {
		if (abs(diag[i]) < 1e-9 * abs(super[i])) { // diag[i] == 0
			b[i+1] -= b[i] * diag[i+1] / super[i];
			if (i+2 <= n) b[i+2] -= b[i] * sub[i+1] / super[i];
			diag[i+1] = sub[i]; tr[++i] = 1;
		} else {
			diag[i+1] -= super[i]*sub[i]/diag[i];
			b[i+1] -= b[i]*sub[i]/diag[i];
		}
	}
	for (int i = n; i >= 1; --i) {
		if (tr[i]) {
			swap(b[i], b[i-1]);
			diag[i-1] = diag[i];
			b[i] /= super[i-1];
		} else {
			b[i] /= diag[i];
			if (i > 1) b[i-1] -= b[i]*super[i-1];
		}
	}
	return b;
}
