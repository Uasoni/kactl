#include "../utilities/template.h"

namespace finite_field {

const int MOD = 7;
// const int INV[] = {0, 1, 3, 2, 4};
const int INV[] = {0, 1, 4, 5, 2, 3, 6};
struct ModularValue {
	int x;
	ModularValue() : x(0) {}
	ModularValue(int y) : x(y % MOD) { if (x < 0) x += MOD; }
};
ModularValue operator+(ModularValue a, ModularValue b) { return {a.x + b.x}; }
ModularValue operator-(ModularValue a, ModularValue b) { return {a.x - b.x}; }
ModularValue operator*(ModularValue a, ModularValue b) { return {a.x * b.x}; }
ModularValue operator/(ModularValue a, ModularValue b) { assert(b.x); return {a.x * INV[b.x]}; }
ModularValue& operator+=(ModularValue& a, ModularValue b) { return a = a + b; }
ModularValue& operator-=(ModularValue& a, ModularValue b) { return a = a - b; }
ModularValue& operator*=(ModularValue& a, ModularValue b) { return a = a * b; }
ModularValue& operator/=(ModularValue& a, ModularValue b) { return a = a / b; }

vector<ModularValue> tridiagonal(vector<ModularValue> diag, const vector<ModularValue>& super,
		const vector<ModularValue>& sub, vector<ModularValue> b) {
	int n = (int)(b).size(); vector<int> tr(n);
	for (int i = 0; i < (n-1); ++i) {
		if (diag[i].x == 0) {
			if (super[i].x == 0) return {};
			if (sub[i].x == 0) return {};
			b[i+1] -= b[i] * diag[i+1] / super[i];
			if (i+2 < n) b[i+2] -= b[i] * sub[i+1] / super[i];
			diag[i+1] = sub[i]; tr[++i] = 1;
		} else {
			diag[i+1] -= super[i]*sub[i]/diag[i];
			b[i+1] -= b[i]*sub[i]/diag[i];
		}
	}
	if (diag[n-1].x == 0) return {};
	for (int i = n; i--;) {
		if (tr[i]) {
			swap(b[i], b[i-1]);
			diag[i-1] = diag[i];
			b[i] /= super[i-1];
		} else {
			b[i] /= diag[i];
			if (i) b[i-1] -= b[i]*super[i-1];
		}
	}
	return b;
}

int mod_inverse(int x) {
	assert(x);
	if (x < 0) x += MOD;
	return INV[x];
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
void rec(ModularValue& b, int& a, F f) {
	for (int i = 0; i < (MOD); ++i) a = i, b = ModularValue(i), f();
}

int main() {
#ifdef BRUTEFORCE
	const int n = 3;
	vector<vector<int>> mat(n, vector<int>(n)), mat2;
	vector<int> b(n), b3, x(n);
	vector<ModularValue> b2(n);
	vector<ModularValue> diag(n);
	vector<ModularValue> super(n-1);
	vector<ModularValue> sub(n-1);
	rec(diag[0], mat[0][0], [&]() {
	rec(diag[1], mat[1][1], [&]() {
	rec(diag[2], mat[2][2], [&]() {
	rec(super[0], mat[0][1], [&]() {
	rec(super[1], mat[1][2], [&]() {
	rec(sub[0], mat[1][0], [&]() {
	rec(sub[1], mat[2][1], [&]() {
	rec(b2[0], b[0], [&]() {
	rec(b2[1], b[1], [&]() {
	rec(b2[2], b[2], [&]() {
#else
	for (int it = 0; it < (1000000); ++it) {
	const int n = 1 + rand() % 10;
	vector<vector<int>> mat(n, vector<int>(n)), mat2;
	vector<int> b(n), b3, x(n);
	vector<ModularValue> b2(n);
	vector<ModularValue> diag(n);
	vector<ModularValue> super(n-1);
	vector<ModularValue> sub(n-1);
	for (int i = 0; i < (n); ++i) {
		diag[i] = ModularValue(mat[i][i] = rand() % MOD);
		b2[i] = ModularValue(b[i] = rand() % MOD);
	}
	for (int i = 0; i < (n-1); ++i) {
		super[i] = ModularValue(mat[i][i+1] = rand() % MOD);
		sub[i] = ModularValue(mat[i+1][i] = rand() % MOD);
	}
#endif
		mat2 = mat;
		b3 = b;
		int r = solve_linear(mat2, b3, x);
		auto x2 = tridiagonal(diag, super, sub, b2);
		if (r != n) {
			assert(x2.empty());
		} else {
			for (int i = 0; i < (n); ++i) if (x2[i].x != (x[i] + MOD) % MOD) {
				goto fail;
			}
			if (false) {
fail:;
				for (int i = 0; i < (n); ++i) {
					for (int j = 0; j < (n); ++j) cout << mat[i][j] << ' ';
					cout << "x = " << b[i];

					cout << "  " << x[i] << "  " << x2[i].x << endl;
				}
				abort();
			}
		}
#ifdef BRUTEFORCE
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
#else
	}
#endif
	return 0;
}

}

namespace real {

typedef double T;
vector<T> tridiagonal(vector<T> diag, const vector<T>& super,
		const vector<T>& sub, vector<T> b) {
	int n = (int)(b).size(); vector<int> tr(n);
	for (int i = 0; i < (n-1); ++i) {
		if (abs(diag[i]) < 1e-9 * abs(super[i])) { // diag[i] == 0
			throw false; // assert that this doesn't happen; we're testing stability
			b[i+1] -= b[i] * diag[i+1] / super[i];
			if (i+2 < n) b[i+2] -= b[i] * sub[i+1] / super[i];
			diag[i+1] = sub[i]; tr[++i] = 1;
		} else {
			diag[i+1] -= super[i]*sub[i]/diag[i];
			b[i+1] -= b[i]*sub[i]/diag[i];
		}
	}
	for (int i = n; i--;) {
		if (tr[i]) {
			swap(b[i], b[i-1]);
			diag[i-1] = diag[i];
			b[i] /= super[i-1];
		} else {
			b[i] /= diag[i];
			if (i) b[i-1] -= b[i]*super[i-1];
		}
	}
	return b;
}

typedef double T;
int solve_linear(vector<vector<double>>& matrix, vector<double>& b,
		vector<double>& x) {
	int n = (int)(matrix).size(), m = (int)(x).size(), rank = 0, br, bc;
	if (n) assert((int)(matrix[0]).size() == m);
	vector<int> col(m); iota(begin(col), end(col), 0);

	for (int i = 0; i < (n); ++i) {
		double v, bv = -1;
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
		bv = 1/matrix[i][i];
		for (int j = i+1; j < (n); ++j) {
			double fac = matrix[j][i] * bv;
			b[j] -= fac * b[i];
			for (int k = i+1; k < (m); ++k) matrix[j][k] -= fac*matrix[i][k];
		}
		rank++;
	}

	x.assign(m, 0);
	for (int i = rank; i--;) {
		b[i] /= matrix[i][i];
		x[col[i]] = b[i];
		for (int j = 0; j < (i); ++j)
			b[j] = (b[j] - matrix[j][i] * b[i]);
	}
	return rank;
}

int positive_definite(vector<vector<double>>& matrix) {
	int n = (int)(matrix).size(), m = n;
	if (n) assert((int)(matrix[0]).size() == m);
	vector<int> col(m); iota(begin(col), end(col), 0);

	for (int i = 0; i < (n); ++i) {
		double v = matrix[i][i];
		if (v < 1e-9) return false;
		double bv = 1/matrix[i][i];
		for (int j = i+1; j < (n); ++j) {
			double fac = matrix[j][i] * bv;
			for (int k = i+1; k < (m); ++k) matrix[j][k] -= fac*matrix[i][k];
		}
	}
	return true;
}

double bf_nice_doubles[] = {-1, 0, 1, 0.5, -0.5, 1/3.0, 2};
double nice_doubles[] = {-1, 0, 0, 1, 0.5, -0.5, 1/3.0, -1/3.0, 2, -2};
double nice_double() {
	return nice_doubles[rand() % (sizeof nice_doubles / sizeof *nice_doubles)];
}

bool valid_mat(const vector<vector<double>>& mat) {
	const int n = (int)(mat).size();
	bool faila = false, failb = false, sym = true;
	for (int i = 0; i < (n); ++i) {
		double suma = 0, sumb = 0;
		for (int j = 0; j < (n); ++j) {
			if (mat[i][j] != mat[j][i]) sym = false;
			suma += abs(mat[i][j]);
			sumb += abs(mat[j][i]);
		}
		if (!suma || suma / 2 >= abs(mat[i][i])) faila = true;
		if (!sumb || sumb / 2 >= abs(mat[i][i])) failb = true;
	}
	if (!faila || !failb) return true;
	if (sym) {
		vector<vector<double>> mat2 = mat;
		return positive_definite(mat2);
	}
	return false;
}

template<class F>
void rec(T& a, T& b, F f) {
	for (double x : bf_nice_doubles) {
		b = a = x;
		f();
	}
}

int main() {
	ll count = 0;
#ifdef BRUTEFORCE
	const int n = 3;
	vector<vector<double>> mat(n, vector<double>(n)), mat2;
	vector<double> b(n), b3, x(n), x2(n);
	vector<T> b2(n);
	vector<T> diag(n);
	vector<T> super(n-1);
	vector<T> sub(n-1);
	rec(diag[0], mat[0][0], [&]() {
	rec(diag[1], mat[1][1], [&]() {
	rec(diag[2], mat[2][2], [&]() {
	rec(super[0], mat[0][1], [&]() {
	rec(super[1], mat[1][2], [&]() {
	rec(sub[0], mat[1][0], [&]() {
	rec(sub[1], mat[2][1], [&]() {
	rec(b2[0], b[0], [&]() {
	rec(b2[1], b[1], [&]() {
	rec(b2[2], b[2], [&]() {
#else
	for (int it = 0; it < (10000000); ++it) {
	const int n = 1 + rand() % 10;
	vector<vector<double>> mat(n, vector<double>(n)), mat2;
	vector<double> b(n), b3, x(n), x2(n);
	vector<T> b2(n);
	vector<T> diag(n);
	vector<T> super(n-1);
	vector<T> sub(n-1);
	for (int i = 0; i < (n); ++i) {
		diag[i] = mat[i][i] = nice_double();
		b2[i] = b[i] = nice_double();
	}
	bool sym = rand() % 3 == 0;
	for (int i = 0; i < (n-1); ++i) {
		double x = nice_double();
		super[i] = mat[i][i+1] = x;
		double y = sym ? x : nice_double();
		sub[i] = mat[i+1][i] = y;
	}
#endif
	{
		if (!valid_mat(mat)) goto skip;
		count++;
		mat2 = mat;
		b3 = b;
		bool done = false;
		try {
			int r = solve_linear(mat2, b3, x);
			x2 = tridiagonal(diag, super, sub, b2);
			assert(r == n);
			done = true;
			for (int i = 0; i < (n); ++i) if (abs(x2[i] - x[i]) > 1e-6) {
				throw false;
			}
		} catch (bool) {
			for (int i = 0; i < (n); ++i) {
				for (int j = 0; j < (n); ++j) cout << mat[i][j] << ' ';
				cout << "x = " << b[i];

				cout << "  " << x[i] << "  ";
				if (done) cout << x2[i];
				else cout << "?";
				cout << endl;
			}
			abort();
		}
	} skip:;
#ifdef BRUTEFORCE
	});
	});
	});
	});
	});
	});
	});
	});
	});
	});
#else
	}
#endif
	cout<<"tests passed!"<<endl;
	return 0;
}

}

int main() {
	// finite_field::main();
	real::main();
}
