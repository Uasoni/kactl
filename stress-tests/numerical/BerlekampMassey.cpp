#include "../utilities/template.h"

const ll MOD = 5;

#include "../../content/numerical/BerlekampMassey.h"

template<class F>
void gen(vector<ll>& v, int at, F f) {
	if (at == (int)(v).size()) f();
	else {
		for (int i = 0; i < MOD; ++i) {
			v[at] = i;
			gen(v, at+1, f);
		}
	}
}

int main() {
	for (int n = 1; n < (5); ++n) {
		vector<ll> start(n);
		vector<ll> coef(n), coef2;
		vector<ll> full(2*n);
		gen(start, 0, [&]() {
		gen(coef, 0, [&]() {
			for (int i = 0; i < (n); ++i) full[i] = start[i];
			for (int i = n; i < (2*n); ++i) full[i] = 0;
			for (int i = n; i < 2*n; ++i) for (int j = 0; j < n; ++j)
				full[i] = (full[i] + coef[j] * full[i-1 - j]) % MOD;
			coef2 = berlekamp_massey(full, MOD);
// for (int i = 0; i < (2*n); ++i) cerr << full[i] << ' '; cerr << endl;
// for (int i = 0; i < (n); ++i) cerr << coef[i] << ' '; cerr << endl;
// for (int i = 0; i < ((int)(coef2).size()); ++i) cerr << coef2[i] << ' '; cerr << endl;
			if ((int)(coef2).size() == n) assert(coef == coef2);
// for (int i = 0; i < (n); ++i) cerr << full[i] << ' ';
			for (int i = n; i < (2*n); ++i) {
				ll x = 0;
				for (int j = 0; j < (int)coef2.size(); ++j)
					x = (x + coef2[j] * full[i-1 - j]) % MOD;
				// cerr << x << ' ';
				assert(x == full[i]);
			}
			// cerr << endl;
// cerr << endl;
		});
		});
	}
	cout<<"tests passed!"<<endl;
	return 0;
}

int main2() {
	vector<ll> v{0, 1, 1, 3, 5, 11};
	auto v2 = berlekamp_massey(v, MOD);
	for(auto &x: v2) cout << x << ' ';
	cout << endl;
	return 0;
}
