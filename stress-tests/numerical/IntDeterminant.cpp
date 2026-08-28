#include "../utilities/template.h"

const int MOD = 7; // 4
ll det(vector<vector<ll>>& a) { // integer determinant
	int n = (int)(a).size(); ll ans = 1;
	for (int i = 0; i < (n); ++i) {
		for (int j = i+1; j < (n); ++j) {
			while (a[j][i] != 0) { // gcd step
				ll t = a[i][i] / a[j][i];
				for (int k = i; k < (n); ++k)
					a[i][k] = (a[i][k] - a[j][k] * t) % MOD;
				swap(a[i], a[j]);
				ans *= -1;
			}
		}
		if (!a[i][i]) return 0;
		ans = ans * a[i][i] % MOD;
	}
	if (ans < 0) ans += MOD;
	return ans;
}

ll idet(vector<vector<ll>>& a) { // integer determinant
	int n = (int)(a).size(); ll ans = 1;
	for (int i = 0; i < (n); ++i) {
		for (int j = i+1; j < (n); ++j) {
			while (a[j][i] != 0) { // gcd step
				ll t = a[i][i] / a[j][i]; // can take MOD-inv if MOD p
				for (int k = i; k < (n); ++k) a[i][k] -= a[j][k] * t;
				swap(a[i], a[j]);
				ans *= -1;
			}
		}
		if (!a[i][i]) return 0;
		ans *= a[i][i];
	}
	return ans;
}

double det(vector<vector<double>>& a) {
	int n = (int)(a).size(); double res = 1;
	for (int i = 0; i < (n); ++i) {
		int b = i;
		for (int j = i+1; j < (n); ++j) if (fabs(a[j][i]) > fabs(a[b][i])) b = j;
		if (i != b) swap(a[i], a[b]), res *= -1;
		res *= a[i][i];
		if (res == 0) return 0;
		for (int j = i+1; j < (n); ++j) {
			double v = a[j][i] / a[i][i];
			if (v != 0) for (int k = i+1; k < (n); ++k) a[j][k] -= v * a[i][k];
		}
	}
	return res;
}

template<class F>
void rec(int i, int j, vector<vector<ll>>& values, F f) {
	if (i == (int)(values).size()) {
		f();
	}
	else if (j == (int)(values[i]).size()) {
		rec(i+1, 0, values, f);
	}
	else {
		for (int v = 0; v < (MOD); ++v) {
			values[i][j] = v;
			rec(i, j+1, values, f);
		}
	}
}

template<class F>
void rec2(int i, vector<ll>& values, F f) {
	if (i == (int)(values).size()) f();
	else {
		for (int v = 0; v < (MOD); ++v) {
			values[i] = v;
			rec2(i+1, values, f);
		}
	}
}

int main() {
	for (int n = 0; n < (4); ++n) {
		vector<vector<ll>> mat(n, vector<ll>(n, 0)), mat2;
		vector<vector<double>> mat3(n, vector<double>(n, 0));
		rec(0,0,mat,[&]() {
			for (int i = 0; i < (n); ++i) for (int j = 0; j < (n); ++j) mat3[i][j] = mat[i][j];
			// mat2 = mat; ll a = det(mat2);
			int a = (int)round(det(mat3)) % MOD;
			mat2 = mat; ll b = idet(mat2) % MOD;
			if (a < 0) a += MOD;
			if (b < 0) b += MOD;
			if (a != b) {
				for (int i = 0; i < (n); ++i) {
					for (int j = 0; j < (n); ++j) cout << mat[i][j];
					cout << endl;
				}
				cout << a << ' ' << b << endl;
				assert(a == b);
			}
		});
	}
	cout<<"tests passed!"<<endl;
}
