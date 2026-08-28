#include "../utilities/template.h"

const int MOD = 3;
const int N_MAX = 4, M_MAX = 4, NM_MAX = 10;

const int lut[9] = {-4,-2,-3,-1,-100,1,3,2,4};

int mod_inverse(int x) {
	assert(x);
	return x;
	// return lut[x+4];
}

int solve_linear(vector<vector<int>>& matrix, vector<int>& b, vector<int>& x) {
	int n = (int)(matrix).size(), m = (int)(x).size(), rank = 0, br, bc;
	if (n) assert((int)(matrix[0]).size() == m);
	vector<int> col(m); iota(begin(col), end(col), 0);

	for (int i = 0; i < (n); ++i) {
		int v, bv = -1;
		for (int r = i; r < (n); ++r) for (int c = i; c < (m); ++c)
			if ((v = matrix[r][c])) {
				br = r, bc = c, bv = v;
				goto found;
			}
		for (int j = i; j < (n); ++j) if (b[j]) return -1;
		break;
found:
		swap(matrix[i], matrix[br]);
		swap(b[i], b[br]);
		swap(col[i], col[bc]);
		for (int j = 0; j < (n); ++j) swap(matrix[j][i], matrix[j][bc]);
		bv = mod_inverse(matrix[i][i]);
		for (int j = i+1; j < (n); ++j) {
			int fac = matrix[j][i] * bv % MOD;
			b[j] = (b[j] - fac * b[i]) % MOD;
			for (int k = i+1; k < (m); ++k) matrix[j][k] = (matrix[j][k] - fac*matrix[i][k]) % MOD;
		}
		rank++;
	}

	x.assign(m, 0);
	for (int i = rank; i--;) {
		b[i] = ((b[i] * mod_inverse(matrix[i][i]) % MOD) + MOD) % MOD;
		x[col[i]] = b[i];
		for (int j = 0; j < (i); ++j)
			b[j] = (b[j] - matrix[j][i] * b[i]);
	}
	return rank;
}

template<class F>
void rec(int i, int j, vector<vector<int>>& matrix, F f) {
	if (i == (int)(matrix).size()) {
		f();
	}
	else if (j == (int)(matrix[i]).size()) {
		rec(i+1, 0, matrix, f);
	}
	else {
		for (int v = 0; v < (MOD); ++v) {
			matrix[i][j] = v;
			rec(i, j+1, matrix, f);
		}
	}
}

template<class F>
void rec2(int i, vector<int>& matrix, F f) {
	if (i == (int)(matrix).size()) f();
	else {
		for (int v = 0; v < (MOD); ++v) {
			matrix[i] = v;
			rec2(i+1, matrix, f);
		}
	}
}

int main() {
	for (int n = 0; n < (N_MAX+1); ++n) for (int m = 0; m < (M_MAX+1); ++m) {
		int nm = n*m;
		if (nm > NM_MAX) continue;
		vector<vector<int>> matrix(n, vector<int>(m));
		vector<int> b(n), x(m), the_x(m);
		rec(0, 0, matrix, [&]() {
			rec2(0, b, [&]() {
				int sols = 0;
				rec2(0, x, [&]() {
					for (int i = 0; i < (n); ++i) {
						int v = 0;
						for (int j = 0; j < (m); ++j) v += matrix[i][j] * x[j];
						if (v % MOD != b[i]) return;
					}
					sols++;
					if (sols == 1) the_x = x;
				});
				vector<vector<int>> matrix2 = matrix;
				vector<int> x2 = x, b2 = b;
				int r = solve_linear(matrix2, b2, x2);
				if (sols == 0) assert(r == -1);
				else if (sols == 1) assert(r == m);
				else assert(r < m);
				if (sols == 1) assert(x2 == the_x);
			});
		});
	}
	cout<<"tests passed!"<<endl;
}
