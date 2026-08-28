/**
 * Author: johan sannemo
 * Date: 2014-11-28
 * License: CC0
 * Source: folklore
 * Description: calculate submatrix sums from a 1-indexed matrix using
 * inclusive-inclusive corners. Row and column 0 are ignored.
 * Usage:
 * SubMatrix<int> m(matrix);
 * m.sum(1, 1, 2, 2); // top left 4 elements
 * Time: O(N^2 + Q)
 * Status: tested on kattis
 */
#pragma once

template<class T>
struct SubMatrix {
	vector<vector<T>> p;
	SubMatrix(vector<vector<T>>& v) {
		int rows = (int)v.size() - 1, columns = (int)v[1].size() - 1;
		p.assign(rows + 1, vector<T>(columns + 1));
		for (int r = 1; r <= rows; ++r) for (int c = 1; c <= columns; ++c)
			p[r][c] = v[r][c] + p[r-1][c] + p[r][c-1] - p[r-1][c-1];
	}
	T sum(int upper, int left, int lower, int right) {
		return p[lower][right] - p[lower][left - 1]
			- p[upper - 1][right] + p[upper - 1][left - 1];
	}
};
