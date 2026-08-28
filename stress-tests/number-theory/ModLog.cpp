#include "../utilities/template.h"

#include "../../content/number-theory/ModLog.h"

int main() {
	const int lim = 100;
	for (int m = 1; m < (lim); ++m) {
		for (int a = 0; a < (lim); ++a) {
			vector<ll> ans(m, -1);
			ll b = a % m;
			for (int x = 1; x < (max(m,2)); ++x) {
				if (ans[b] == -1) ans[b] = x;
				b = b * a % m;
			}
			for (int b = 0; b < (m); ++b) {
				ll res = mod_log(a, b, m);
				if (ans[b] != res) {
					cerr << "FAIL" << endl;
					cerr << "expected log(" << a << ", " << b << ", " << m << ") = " << ans[b] << ", found " << res << endl;
					return 1;
				}
			}
		}
	}
	cout<<"tests passed!"<<endl;
}
