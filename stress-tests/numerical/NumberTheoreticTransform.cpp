#include "../utilities/template.h"
#include "../../content/numerical/NumberTheoreticTransform.h"
ll reference_mod_pow(ll a, ll e) {
	if (e == 0)
		return 1;
	ll x = reference_mod_pow(a * a % MOD, e >> 1);
	return e & 1 ? x * a % MOD : x;
}

vector<ll> simple_conv(vector<ll> a, vector<ll> b) {
	int s = (int)(a).size() + (int)(b).size() - 1;
	if (a.empty() || b.empty()) return {};
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
	ll res = 0, res2 = 0;
	int ind = 0, ind2 = 0;
	vector<ll> a, b;
	for (int it = 0; it < (6000); ++it) {
		a.resize(ra() % 10);
		b.resize(ra() % 10);
		for(auto &x: a) x = (ra() % 100 - 50 + MOD) % MOD;
		for(auto &x: b) x = (ra() % 100 - 50 + MOD) % MOD;
		for(auto &x: simple_conv(a, b)) res += (ll)x * ind++ % MOD;
		for(auto &x: conv(a, b)) res2 += (ll)x * ind2++ % MOD;
		a.resize(16);
			vector<ll> a2 = a;
			ntt(a2);
			for (int k = 0; k < ((int)(a2).size()); ++k) {
				ll sum = 0;
				for (int x = 0; x < ((int)(a2).size()); ++x) {
					sum = (sum + a[x] * reference_mod_pow(ROOT,
						k * x * (MOD - 1) / (int)a.size())) % MOD;
				}
				assert(sum == a2[k]);
			}
	}
	assert(res==res2);
	cout<<"tests passed!"<<endl;
}
