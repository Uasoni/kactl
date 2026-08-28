#include "../utilities/template.h"

#include "../../content/strings/KMP.h"

template<class F>
void gen(string& s, int at, int alpha, F f) {
	if (at == (int)(s).size()) f();
	else {
		for (int i = 0; i < (alpha); ++i) {
			s[at] = (char)('a' + i);
			gen(s, at+1, alpha, f);
		}
	}
}

void test(const string& s) {
	vector<int> p = pi(s);
	for (int i = 0; i < ((int)(s).size()); ++i) {
		int maxlen = -1;
		for (int len = 0; len < (i+1); ++len) {
			for (int j = 0; j < (len); ++j) {
				if (s[j] != s[i+1 - len + j]) goto fail;
			}
			maxlen = len;
fail:;
		}
		assert(maxlen == p[i]);
	}
}

int main() {
	// string str; cin >> str; for(auto &x: pi(str)) cout << x; cout << endl;
	// test ~3^12 strings
	for (int n = 0; n < (13); ++n) {
		string s(n, 'x');
		gen(s, 0, 3, [&]() {
			test(s);
		});
	}
	// then ~4^10 strings
	for (int n = 0; n < (11); ++n) {
		string s(n, 'x');
		gen(s, 0, 4, [&]() {
			test(s);
		});
	}
	cout<<"tests passed!"<<endl;
}
