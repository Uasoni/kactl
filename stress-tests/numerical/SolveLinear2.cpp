#include "../utilities/template.h"
const double EPS = 1e-12;

enum { YES, NO, MULT };
int solve_linear(vector<vector<double>>& matrix, vector<double>& b, vector<double>& x) {
	int n = (int)(matrix).size(), m = (int)(x).size(), br = -1, bc = -1;
	vector<int> col(m); iota(begin(col), end(col), 0);

	for (int i = 0; i < (n); ++i) {
		double v, bv = -1;
		for (int r = i; r < (n); ++r) for (int c = i; c < (m); ++c)
			if ((v = fabs(matrix[r][c])) > bv)
				br = r, bc = c, bv = v;
		if (bv <= EPS) {
			for (int j = i; j < (n); ++j) if (fabs(b[j]) > EPS) return NO;
			if (i == m) break;
			return MULT;
		}
		swap(matrix[i], matrix[br]);
		swap(b[i], b[br]);
		swap(col[i], col[bc]);
		for (int j = 0; j < (n); ++j) swap(matrix[j][i], matrix[j][bc]);
		bv = 1/matrix[i][i];
		for (int j = i+1; j < (n); ++j) {
			double fac = matrix[j][i] * bv;
			b[j] -= fac * b[i];
			for (int k = i+1; k < (m); ++k) matrix[j][k] -= fac*matrix[i][k];
		}
	}
	if (n < m) return MULT;

	for (int i = m; i--;) {
		x[col[i]] = (b[i] /= matrix[i][i]);
		for (int j = 0; j < (i); ++j)
			b[j] -= matrix[j][i] * b[i];
	}
	return YES;
}

int main() {
	const int n = 1000;
	vector<vector<double>> matrix(n, vector<double>(n));
	for (int i = 0; i < (n); ++i) for (int j = 0; j < (n); ++j) matrix[i][j] = rand() * 1000.0 / RAND_MAX;
	vector<double> x(n), b(n);
	for (int i = 0; i < (n); ++i) b[i] = rand() * 1000.0 / RAND_MAX;
	int r = solve_linear(matrix, b, x);
	assert(r == 0);
	cout<<"tests passed!"<<endl;
	// cout << r << endl;
	// for (int i = 0; i < (n); ++i) cout << x[i] << ' ';
	// cout << endl;
}
