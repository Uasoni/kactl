#include "../utilities/template.h"

bool valid(vector<int> deg) {
	int n = (int)(deg).size();
	sort(begin(deg), end(deg));
	reverse(begin(deg), end(deg));
	int sum = 0;
	for (int i = 0; i < (n); ++i) sum += deg[i];
	if (sum & 1) return 0;
	for (int k = 0; k < (n); ++k) {
		int s = 0, t = 0;
		for (int i = 0; i < (k+1); ++i)
			s += deg[i];
		for (int i = k+1; i < (n); ++i)
			t += min(deg[i], k + 1);
		if (s > k * (k+1) + t) return 0;
	}
	return 1;
}

int main() {
	for (int n = 0; n < (7); ++n) {
		vector<pii> possible_ed;
		for (int i = 0; i < (n); ++i) for (int j = 0; j < (i); ++j) possible_ed.emplace_back(i, j);
		set<vector<int>> valids;
		for (int bi = 0; bi < ((1 << (int)(possible_ed).size())); ++bi) {
			vector<int> deg(n);
			for (int i = 0; i < ((int)(possible_ed).size()); ++i) if (bi & (1 << i)) {
				int a, b;
				tie(a, b) = possible_ed[i];
				deg[a]++;
				deg[b]++;
			}
			assert(valid(deg));
			valids.insert(deg);
		}

		vector<int> de(n);
		function<void(int)> rec = [&](int at) {
			if (at == n) {
				assert(valid(de) == valids.count(de));
			} else {
				for (int a = 0; a < (n); ++a) {
					de[at] = a;
					rec(at + 1);
				}
			}
		};
		rec(0);
	}
	cout << "tests passed!" << endl;
}
