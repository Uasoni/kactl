/**
 * Author: simon lindholm
 * Date: 2016-12-09
 * License: CC0
 * Source: http://www.mimuw.edu.pl/~mucha/pub/mucha_sankowski_focs04.pdf
 * Description: matching for general graphs.
 * fails with probability $N / MOD$.
 * Time: O(N^3)
 * Status: not very well tested
 */
#pragma once

#include "../numerical/MatrixInverse-mod.h"

vector<pii> general_matching(int n, vector<pii>& edges) {
	vector<vector<ll>> mat(n + 1, vector<ll>(n + 1)), inverse;
	for (pii pa : edges) {
		int a = pa.first, b = pa.second, r = rand() % MOD;
		mat[a][b] = r, mat[b][a] = (MOD - r) % MOD;
	}

	int r = mat_inv(inverse = mat), size = 2*n - r, fi, fj;
	assert(r % 2 == 0);

	if (size != n) do {
		mat.resize(size + 1, vector<ll>(size + 1));
		for (int i = 1; i <= n; ++i) {
			mat[i].resize(size + 1);
			for (int j = n + 1; j <= size; ++j) {
				int r = rand() % MOD;
				mat[i][j] = r, mat[j][i] = (MOD - r) % MOD;
			}
		}
	} while (mat_inv(inverse = mat) != size);

	vector<int> has(size + 1, 1); vector<pii> ret;
	for (int it = 0; it < size/2; ++it) {
		for (int i = 1; i <= size; ++i) if (has[i])
			for (int j = i+1; j <= size; ++j) if (inverse[i][j] && mat[i][j]) {
				fi = i; fj = j; goto done;
		} assert(0); done:
		if (fj <= n) ret.emplace_back(fi, fj);
		has[fi] = has[fj] = 0;
		for (int sw = 0; sw < (2); ++sw) {
			ll a = mod_pow(inverse[fi][fj], MOD - 2);
			for (int i = 1; i <= size; ++i) if (has[i] && inverse[i][fj]) {
				ll b = inverse[i][fj] * a % MOD;
				for (int j = 1; j <= size; ++j)
					inverse[i][j] = (inverse[i][j] - inverse[fi][j] * b) % MOD;
			}
			swap(fi,fj);
		}
	}
	return ret;
}
