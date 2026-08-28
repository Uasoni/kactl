/**
 * Author: per austrin, simon lindholm
 * Date: 2004-02-08
 * License: CC0
 * Description: solves $matrix * x = b$ for 1-indexed inputs. if there are
 *  multiple solutions, an arbitrary one is returned. returns rank, or -1 if
 *  no solutions. data in $matrix$ and $b$ is lost; x has index 0 unused.
 * Time: O(n^2 m)
 * Status: tested on kattis:equationsolver, and BruteForce-tested mod 3 and 5 for n,m <= 3
 */
#pragma once
const double EPS = 1e-12;

int solve_linear(vector<vector<double>>& matrix, vector<double>& b, vector<double>& x) {
	int n = (int)matrix.size() - 1;
	int m = n ? (int)matrix[1].size() - 1 : 0;
	int rank = 0, br = 0, bc = 0;
	vector<int> col(m + 1); iota(next(begin(col)), end(col), 1);

	for (int i = 1; i <= min(n, m); ++i) {
		double v, bv = 0;
		for (int r = i; r <= n; ++r) for (int c = i; c <= m; ++c)
			if ((v = fabs(matrix[r][c])) > bv)
				br = r, bc = c, bv = v;
		if (bv <= EPS) {
			for (int j = i; j <= n; ++j) if (fabs(b[j]) > EPS) return -1;
			break;
		}
		swap(matrix[i], matrix[br]);
		swap(b[i], b[br]);
		swap(col[i], col[bc]);
		for (int j = 1; j <= n; ++j) swap(matrix[j][i], matrix[j][bc]);
		bv = 1/matrix[i][i];
		for (int j = i+1; j <= n; ++j) {
			double fac = matrix[j][i] * bv;
			b[j] -= fac * b[i];
			for (int k = i+1; k <= m; ++k) matrix[j][k] -= fac*matrix[i][k];
		}
		rank++;
	}

	for (int i = rank + 1; i <= n; ++i) if (fabs(b[i]) > EPS) return -1;
	x.assign(m + 1, 0);
	for (int i = rank; i >= 1; --i) {
		b[i] /= matrix[i][i];
		x[col[i]] = b[i];
		for (int j = 1; j < i; ++j) b[j] -= matrix[j][i] * b[i];
	}
	return rank; // (multiple solutions if rank < m)
}
