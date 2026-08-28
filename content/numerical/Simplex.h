/**
 * Author: stanford
 * Source: stanford notebook
 * License: MIT
 * Description: solves a general linear maximization problem: maximize $c^T x$ subject to $ax \le b$, $x \ge 0$.
 * returns -INF if there is no solution, INF if there are arbitrarily good solutions, or the maximum value of $c^T x$ otherwise.
 * matrix, b, c and the returned x are 1-indexed with index 0 unused. the output
 * vector is set to an optimal $x$ (or in the unbounded case, an arbitrary solution fulfilling the constraints).
 * numerical stability is not guaranteed. for better performance, define variables such that $x = 0$ is viable.
 * Usage:
 * vector<vector<scalar>> matrix = {{}, {0,1,-1}, {0,-1,1}, {0,-1,-2}};
 * vector<scalar> b = {0,1,1,-4}, c = {0,-1,-1}, x;
 * scalar val = LinearProgramSolver(matrix, b, c).solve(x);
 * Time: O(NM * \#pivots), where a pivot may be e.g. an edge relaxation. O(2^n) in the general case.
 * Status: seems to work?
 */
#pragma once

typedef double scalar; // long double, rational, double + mod<P>...

const scalar EPS = 1e-8, INF = 1/.0;

struct LinearProgramSolver {
	private:
	int m, n;
	vector<int> non_basic, basic;
	vector<vector<scalar>> tableau;

	void pivot(int r, int s) {
		scalar *a = tableau[r].data(), inv = 1 / a[s];
		for (int i = 0; i < (m+2); ++i) if (i != r && abs(tableau[i][s]) > EPS) {
			scalar *b = tableau[i].data(), inv2 = b[s] * inv;
			for (int j = 0; j < (n+2); ++j) b[j] -= a[j] * inv2;
			b[s] = a[s] * inv2;
		}
		for (int j = 0; j < (n+2); ++j) if (j != s) tableau[r][j] *= inv;
		for (int i = 0; i < (m+2); ++i) if (i != r) tableau[i][s] *= -inv;
		tableau[r][s] = inv;
		swap(basic[r], non_basic[s]);
	}

	bool simplex(int phase) {
		int x = m + phase - 1;
		for (;;) {
			int s = -1;
			for (int j = 0; j < n + 1; ++j) if (non_basic[j] != -phase) {
				if (s == -1 || make_pair(tableau[x][j], non_basic[j])
						< make_pair(tableau[x][s], non_basic[s])) s = j;
			}
			if (tableau[x][s] >= -EPS) return true;
			int r = -1;
			for (int i = 0; i < (m); ++i) {
				if (tableau[i][s] <= EPS) continue;
				if (r == -1 || make_pair(tableau[i][n+1] / tableau[i][s], basic[i])
						< make_pair(tableau[r][n+1] / tableau[r][s], basic[r])) r = i;
			}
			if (r == -1) return false;
			pivot(r, s);
		}
	}

	public:
	LinearProgramSolver(const vector<vector<scalar>>& matrix,
			const vector<scalar>& b, const vector<scalar>& c) :
		m((int)b.size() - 1), n((int)c.size() - 1), non_basic(n+1),
		basic(m), tableau(m+2, vector<scalar>(n+2)) {
		for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j)
			tableau[i][j] = matrix[i+1][j+1];
		for (int i = 0; i < m; ++i) {
			basic[i] = n+i; tableau[i][n] = -1; tableau[i][n+1] = b[i+1];
		}
		for (int j = 0; j < n; ++j) {
			non_basic[j] = j; tableau[m][j] = -c[j+1];
		}
		non_basic[n] = -1; tableau[m+1][n] = 1;
	}

	scalar solve(vector<scalar>& x) {
		int r = 0;
		for (int i = 1; i < (m); ++i) if (tableau[i][n+1] < tableau[r][n+1]) r = i;
		if (tableau[r][n+1] < -EPS) {
			pivot(r, n);
			if (!simplex(2) || tableau[m+1][n+1] < -EPS) return -INF;
			for (int i = 0; i < (m); ++i) if (basic[i] == -1) {
				int s = 0;
				for (int j = 1; j < n + 1; ++j)
					if (make_pair(tableau[i][j], non_basic[j])
							< make_pair(tableau[i][s], non_basic[s])) s = j;
				pivot(i, s);
			}
		}
		bool ok = simplex(1); x = vector<scalar>(n + 1);
		for (int i = 0; i < m; ++i) if (basic[i] < n)
			x[basic[i] + 1] = tableau[i][n+1];
		return ok ? tableau[m][n+1] : INF;
	}
};
