#include "../utilities/template.h"

#include "../../content/numerical/LinearRecurrence.h"

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
		vector<ll> coef(n);
		int size = 10*n + 3;
		vector<ll> full(size);
		gen(start,0,[&]() {
			gen(coef,0,[&]() {
				for(auto &x:full) x = 0;
				for (int i = 0; i < (n); ++i) full[i] = start[i];
				for (int i = n; i < size; ++i) for (int j = 0; j < n; ++j)
					full[i] = (full[i] + coef[j] * full[i-1 - j]) % MOD;
	// for (int i = 0; i < (size); ++i) cerr << full[i] << ' '; cerr << endl;
	// for (int i = 0; i < (n); ++i) cerr << coef[i] << ' '; cerr << endl;
	// linear_rec lr(start, coef);
	// for (int i = 0; i < (size); ++i) { cerr << lr.get(i) << ' '; } cerr << endl;
				for (int i = 0; i < (size); ++i) {
					auto v = linear_rec(start, coef, i);
	// cerr << v << ' ';
					assert(v == full[i]);
				}
	// cerr << endl;
	// cerr << endl;
			});
		});
	}
	cout<<"tests passed!"<<endl;
}
