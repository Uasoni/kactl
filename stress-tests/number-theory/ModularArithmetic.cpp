#include "../utilities/template.h"

#include "../../content/number-theory/ModularArithmetic.h"

int main() {
	for (int a = 0; a < MOD; ++a) for (int b = 1; b < MOD; ++b) {
		ModInt ma(a);
		ModInt mb(b);
		ModInt mc = ma / mb;
		assert((mc * mb).x == a);
	}
	ModInt a = 2;
	ll cur=1;
	for (int i = 0; i < (17); ++i) {
		assert((a ^ i).x == cur);
		cur = (cur * 2) % MOD;
		// cout << i << ": " << (a ^ i).x << endl;
	}
	cout<<"tests passed!"<<endl;
}
