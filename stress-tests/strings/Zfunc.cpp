#include "../utilities/template.h"

#include "../../content/strings/Zfunc.h"

template <class F> void gen(string &s, int at, int alpha, F f) {
	if (at == (int)(s).size())
		f();
	else {
		for (int i = 0; i < (alpha); ++i) {
			s[at] = (char)('a' + i);
			gen(s, at + 1, alpha, f);
		}
	}
}

void test(const string &s) {
	int n = (int)(s).size();
	vector<int> found = z_function(s);
	vector<int> expected(n, 0);
	for (int i = 1; i < (n); ++i) { // exclude index 0 (!)
		int j = 0;
		while (i + j < n && s[i + j] == s[j])
			j++;
		expected[i] = j;
	}
	assert(found == expected);
}

signed main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	for (int n = 0; n < (13); ++n) {
		string s(n, 'x');
		gen(s, 0, 3, [&]() { test(s); });
	}
	for (int n = 0; n < (11); ++n) {
		string s(n, 'x');
		gen(s, 0, 4, [&]() { test(s); });
	}
	cout<<"tests passed!"<<endl;
}
