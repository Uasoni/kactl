/**
 * Author: simon lindholm
 * Date: 2016-08-27
 * License: CC0
 * Source: own work
 * Description: solves $ax = b$ over $\mathbb F_2$ using 1-indexed rows and
 *  columns. if there are multiple solutions, one is returned arbitrarily.
 *  returns rank, or -1 if no solutions. destroys $matrix$ and $b$.
 * Time: O(n^2 m)
 * Status: BruteForce-tested for n, m <= 4
 */
#pragma once

int solve_linear(vector<bitset<1000>>& matrix, vector<int>& b, bitset<1000>& x, int m) {
	int n = (int)matrix.size() - 1, rank = 0, br;
	assert(m < (int)x.size());
	vector<int> col(m + 1); iota(next(begin(col)), end(col), 1);
	for (int i = 1; i <= min(n, m); ++i) {
		for (br=i; br<=n; ++br) if ((matrix[br] >> i).any()) break;
		if (br > n) {
			for (int j = i; j <= n; ++j) if(b[j]) return -1;
			break;
		}
		int bc = (int)matrix[br]._Find_next(i-1);
		swap(matrix[i], matrix[br]);
		swap(b[i], b[br]);
		swap(col[i], col[bc]);
		for (int j = 1; j <= n; ++j) if (matrix[j][i] != matrix[j][bc]) {
			matrix[j].flip(i); matrix[j].flip(bc);
		}
		for (int j = i+1; j <= n; ++j) if (matrix[j][i]) {
			b[j] ^= b[i];
			matrix[j] ^= matrix[i];
		}
		rank++;
	}

	x = bitset<1000>();
	for (int i = rank + 1; i <= n; ++i) if (b[i]) return -1;
	for (int i = rank; i >= 1; --i) {
		if (!b[i]) continue;
		x[col[i]] = 1;
		for (int j = 1; j < i; ++j) b[j] ^= matrix[j][i];
	}
	return rank; // (multiple solutions if rank < m)
}
