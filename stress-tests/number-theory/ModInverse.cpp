#include "../utilities/template.h"

ll mod_pow(ll a, ll e, ll mod) {
	if (e == 0) return 1;
	ll x = mod_pow(a * a % mod, e >> 1, mod);
	return e & 1 ? x * a % mod : x;
}
bool is_prime(int x) {
	if (x <= 1) return false;
	for (int i = 2; i*i <= x; ++i) {
		if (x % i == 0) return false;
	}
	return true;
}
int main() {
	for (int it = 1; it < (1000); ++it) {
		int mod = it, LIM=1000;
		if (!is_prime(mod)) continue;
		#include "../../content/number-theory/ModInverse.h"
		for (int i=1; i<it; i++){
			assert(inv[i] == mod_pow(i, mod-2, mod));
		}
	}
	cout<<"tests pass!"<<endl;
}
