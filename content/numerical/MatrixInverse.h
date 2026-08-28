/**
 * Author: max bennedich
 * Date: 2004-02-08
 * Description: invert a 1-indexed matrix. returns rank; result is stored in
 * $matrix$ unless singular (rank < n). Row and column 0 are ignored.
 * can easily be extended to prime moduli; for prime powers, repeatedly
 * set $matrix^{-1} = matrix^{-1} (2I - AA^{-1})\  (\text{mod }p^k)$ where $matrix^{-1}$ starts as
 * the inverse of matrix mod p, and k is doubled in each step.
 * Time: O(n^3)
 * Status: slightly tested
 */
#pragma once

int mat_inv(vector<vector<double>>& matrix) {
	int n = (int)matrix.size() - 1; vector<int> col(n + 1);
	vector<vector<double>> tmp(n + 1, vector<double>(n + 1));
	for (int i = 1; i <= n; ++i) tmp[i][i] = 1, col[i] = i;

	for (int i = 1; i <= n; ++i) {
		int r = i, c = i;
		for (int j = i; j <= n; ++j) for (int k = i; k <= n; ++k)
			if (fabs(matrix[j][k]) > fabs(matrix[r][c]))
				r = j, c = k;
		if (fabs(matrix[r][c]) < 1e-12) return i - 1;
		matrix[i].swap(matrix[r]); tmp[i].swap(tmp[r]);
		for (int j = 1; j <= n; ++j)
			swap(matrix[j][i], matrix[j][c]), swap(tmp[j][i], tmp[j][c]);
		swap(col[i], col[c]);
		double v = matrix[i][i];
		for (int j = i+1; j <= n; ++j) {
			double f = matrix[j][i] / v;
			matrix[j][i] = 0;
			for (int k = i+1; k <= n; ++k) matrix[j][k] -= f*matrix[i][k];
			for (int k = 1; k <= n; ++k) tmp[j][k] -= f*tmp[i][k];
		}
		for (int j = i+1; j <= n; ++j) matrix[i][j] /= v;
		for (int j = 1; j <= n; ++j) tmp[i][j] /= v;
		matrix[i][i] = 1;
	}

	/// forget matrix at this Point, just eliminate tmp backward
	for (int i = n; i > 1; --i) for (int j = 1; j < i; ++j) {
		double v = matrix[j][i];
		for (int k = 1; k <= n; ++k) tmp[j][k] -= v*tmp[i][k];
	}

	for (int i = 1; i <= n; ++i) for (int j = 1; j <= n; ++j)
		matrix[col[i]][col[j]] = tmp[i][j];
	return n;
}
