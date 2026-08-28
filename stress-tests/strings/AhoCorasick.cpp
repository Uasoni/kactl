#include "../utilities/template.h"
#include "../../content/strings/AhoCorasick.h"

template<class F>
void gen(string& s, int at, int alpha, F f) {
	if (at == (int)(s).size()) f();
	else {
		for (int i = 0; i < (alpha); ++i) {
			s[at] = (char)('A' + i);
			gen(s, at+1, alpha, f);
		}
	}
}

void test(const string& s) {
	vector<string> pats;
	string cur;
	for (int i = 0; i < ((int)(s).size()); ++i) {
		if (s[i] == 'A') {
			pats.push_back(cur);
			cur = "";
		}
		else cur += s[i];
	}

	string hay = cur;
	for (const string& pattern : pats) if (pattern.empty()) return;

	AhoCorasick ac(pats);
	vector<vector<int>> positions = ac.find_all(pats, hay);

	vector<int> ord;
	for (int i = 0; i < ((int)(hay).size()); ++i) {
		ord.clear();
		for (int j = 0; j < ((int)(pats).size()); ++j) {
			string& pat = pats[j];
			if (hay.substr(i, pat.size()) == pat) {
				ord.push_back(j);
			}
		}
		sort(begin(positions[i]), end(positions[i]));

		if (positions[i] != ord) {
			cerr << "failed!" << endl;
			cerr << hay << endl;
			for (const string& pattern : pats) cerr << pattern << endl;
			cerr << "failed at position " << i << endl;
			cerr << "got:" << endl;
			for (int position : positions[i]) cerr << position << ' ';
			cerr << endl;
			cerr << "expected:" << endl;
			for (int expected : ord) cerr << expected << ' ';
			cerr << endl;
			abort();
		}
	}
}

int main() {
	// test ~4^10 strings
	for (int n = 0; n < (11); ++n) {
		string s(n, 'x');
		gen(s, 0, 4, [&]() {
			test(s);
		});
	}
	// then ~5^7
	for (int n = 0; n < (8); ++n) {
		string s(n, 'x');
		gen(s, 0, 5, [&]() {
			test(s);
		});
	}
	cout<<"tests passed!"<<endl;
}
