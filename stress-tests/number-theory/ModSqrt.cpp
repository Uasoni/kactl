#include "../utilities/template.h"

ll mod_pow(ll a, ll e, ll mod) {
	if (e == 0) return 1;
	ll x = mod_pow(a * a % mod, e >> 1, mod);
	return e & 1 ? x * a % mod : x;
}

#include "../../content/number-theory/ModSqrt.h"

int main() {
	for (int p = 2; p < (10000); ++p) {
		for (int i = 2; i < (p); ++i) if (p % i == 0) goto next;
		for (int a = 0; a < (p); ++a) {
			if (p != 2 && mod_pow(a, (p-1)/2, p) == p-1) continue;
			ll x = sqrt(a, p);
			assert(0 <= x && x < p);
			assert(x * x % p == a);
		}
next:;
	}
	cout<<"tests passed!"<<endl;
}
