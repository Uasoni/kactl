/**
 * Author: simon lindholm
 * Date: 2016-12-08
 * Source: the regular matrix inverse code
 * Description: invert a 1-indexed matrix $matrix$ modulo a prime. Row and
 * column 0 are ignored.
 * returns rank; result is stored in $matrix$ unless singular (rank < n).
 * for prime powers, repeatedly set $matrix^{-1} = matrix^{-1} (2I - AA^{-1})\  (\text{mod }p^k)$ where $matrix^{-1}$ starts as
 * the inverse of matrix mod p, and k is doubled in each step.
 * Time: O(n^3)
 * Status: slightly tested
 */
#pragma once

#include "../number-theory/ModPow.h"

int mat_inv(vector<vector<ll>>& matrix) {
	int n = (int)matrix.size() - 1; vector<int> col(n + 1);
	vector<vector<ll>> tmp(n + 1, vector<ll>(n + 1));
	for (int i = 1; i <= n; ++i) tmp[i][i] = 1, col[i] = i;

	for (int i = 1; i <= n; ++i) {
		int r = i, c = i;
		for (int j = i; j <= n; ++j) for (int k = i; k <= n; ++k) if (matrix[j][k]) {
			r = j; c = k; goto found;
		}
		return i - 1;
found:
		matrix[i].swap(matrix[r]); tmp[i].swap(tmp[r]);
		for (int j = 1; j <= n; ++j)
			swap(matrix[j][i], matrix[j][c]), swap(tmp[j][i], tmp[j][c]);
		swap(col[i], col[c]);
		ll v = mod_pow(matrix[i][i], MOD - 2);
		for (int j = i+1; j <= n; ++j) {
			ll f = matrix[j][i] * v % MOD;
			matrix[j][i] = 0;
			for (int k = i+1; k <= n; ++k) matrix[j][k] = (matrix[j][k] - f*matrix[i][k]) % MOD;
			for (int k = 1; k <= n; ++k) tmp[j][k] = (tmp[j][k] - f*tmp[i][k]) % MOD;
		}
		for (int j = i+1; j <= n; ++j) matrix[i][j] = matrix[i][j] * v % MOD;
		for (int j = 1; j <= n; ++j) tmp[i][j] = tmp[i][j] * v % MOD;
		matrix[i][i] = 1;
	}

	for (int i = n; i > 1; --i) for (int j = 1; j < i; ++j) {
		ll v = matrix[j][i];
		for (int k = 1; k <= n; ++k) tmp[j][k] = (tmp[j][k] - v*tmp[i][k]) % MOD;
	}

	for (int i = 1; i <= n; ++i) for (int j = 1; j <= n; ++j)
		matrix[col[i]][col[j]] = tmp[i][j] % MOD + (tmp[i][j] < 0)*MOD;
	return n;
}
