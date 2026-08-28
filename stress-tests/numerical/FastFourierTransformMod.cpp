#include "../utilities/template.h"

const ll MOD = 1000000007;

#include "../../content/numerical/FastFourierTransformMod.h"

vector<ll> simple_conv(vector<ll> a, vector<ll> b) {
	if (a.empty() || b.empty()) return {};
	int s = (int)(a).size() + (int)(b).size() - 1;
	vector<ll> c(s);
	for (int i = 0; i < ((int)(a).size()); ++i) for (int j = 0; j < ((int)(b).size()); ++j)
		c[i+j] = (c[i+j] + (ll)a[i] * b[j]) % MOD;
	for(auto &x: c) if (x < 0) x += MOD;
	return c;
}

int ra() {
	static unsigned random_state;
	random_state *= 123671231;
	random_state += 1238713;
	random_state ^= 1237618;
	return random_state >> 1;
}

int main() {
	vector<ll> a, b;
	for (int it = 0; it < (6000); ++it) {
		a.resize(ra() % 100);
		b.resize(ra() % 100);
		for(auto &x: a) x = ra() % MOD;
		for(auto &x: b) x = ra() % MOD;
		auto v1 = simple_conv(a, b);
		auto v2 = conv_mod<MOD>(a, b);
		assert(v1 == v2);
	}
	cout<<"tests passed!"<<endl;
}
