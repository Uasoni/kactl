#include "../utilities/template.h"

const int N_MAX = 5, M_MAX = 5, NM_MAX = 16;

int solve_linear(vector<bitset<5>>& matrix, vector<int>& b, bitset<5>& x, int m) {
	int n = (int)(matrix).size(), rank = 0, br;
	assert(m <= (int)(x).size());
	vector<int> col(m); iota(begin(col), end(col), 0);
	for (int i = 0; i < (n); ++i) {
		for (br=i; br<n; ++br) if (matrix[br].any()) break;
		if (br == n) {
			for (int j = i; j < (n); ++j) if(b[j]) return -1;
			break;
		}
		int bc = (int)matrix[br]._Find_next(i-1);
		swap(matrix[i], matrix[br]);
		swap(b[i], b[br]);
		swap(col[i], col[bc]);
		for (int j = 0; j < (n); ++j) if (matrix[j][i] != matrix[j][bc]) {
			matrix[j].flip(i); matrix[j].flip(bc);
		}
		for (int j = i+1; j < (n); ++j) if (matrix[j][i]) {
			b[j] ^= b[i];
			matrix[j] ^= matrix[i];
		}
		rank++;
	}

	x = bitset<5>();
	for (int i = rank; i--;) {
		if (!b[i]) continue;
		x[col[i]] = 1;
		for (int j = 0; j < (i); ++j) b[j] ^= matrix[j][i];
	}
	return rank; // (multiple solutions if rank < m)
}

template<class F>
void rec(int i, int j, vector<bitset<5>>& matrix, int m, F f) {
	if (i == (int)(matrix).size()) {
		f();
	}
	else if (j == m) {
		rec(i+1, 0, matrix, m, f);
	}
	else {
		for (int v = 0; v < (2); ++v) {
			matrix[i][j] = v;
			rec(i, j+1, matrix, m, f);
		}
	}
}

template<class F>
void rec2(int i, bitset<5>& matrix, int m, F f) {
	if (i == m) f();
	else {
		for (int v = 0; v < (2); ++v) {
			matrix[i] = v;
			rec2(i+1, matrix, m, f);
		}
	}
}

int main() {
	int ct = 0;
	for (int n = 0; n < (N_MAX+1); ++n) for (int m = 0; m < (M_MAX+1); ++m) {
		int nm = n*m;
		if (nm > NM_MAX) continue;
		vector<bitset<5>> matrix(n, bitset<5>(m));
		bitset<5> b, x, the_x;
		vector<int> b2(n);
		rec(0, 0, matrix, m, [&]() {
			rec2(0, b, n, [&]() {
				int sols = 0;
				rec2(0, x, m, [&]() {
					for (int i = 0; i < (n); ++i) {
						int v = 0;
						for (int j = 0; j < (m); ++j) v ^= matrix[i][j] & x[j];
						if (v != b[i]) return;
					}
					sols++;
					if (sols == 1) the_x = x;
				});
				vector<bitset<5>> matrix2 = matrix;
				bitset<5> x2 = x; for (int i = 0; i < (n); ++i) b2[i] = b[i];
				int r = solve_linear(matrix2, b2, x2, m);
				if (sols == 0) assert(r == -1);
				else if (sols == 1) assert(r == m);
				else assert(r < m);
				if (sols == 1) assert(x2 == the_x);
				ct++;
			});
		});
	}
	cout<<"tests passed!"<<endl;
}
