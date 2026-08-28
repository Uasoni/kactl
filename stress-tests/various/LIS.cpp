#include "../utilities/template.h"

#include "../../content/various/LIS.h"

template<class I> vector<int> lis_weak(const vector<I>& values) {
	if (values.empty()) return {};
	vector<int> prev((int)(values).size());
	vector<pair<I, int>> res;
	for (int i = 0; i < ((int)(values).size()); ++i) {
		// 0 -> i for longest non-decreasing subsequence
		auto it = lower_bound(begin(res), end(res), pair<I, int>{values[i], i});
		if (it == res.end()) res.emplace_back(), it = res.end()-1;
		*it = {values[i], i};
		prev[i] = it == res.begin() ? 0 : (it-1)->second;
	}
	int length = (int)(res).size(), cur = res.back().second;
	vector<int> ans(length);
	while (length--) ans[length] = cur, cur = prev[cur];
	return ans;
}

int main() {
	for (int weak = 0; weak < (2); ++weak) {
		auto lt = [weak](int a, int b) { return weak ? a <= b : a < b; };
		for (int it = 0; it < (1000000); ++it) {
			int n = rand() % 7;
			vector<int> v(n);
			for(auto &x: v) x = rand() % 4;
			vector<int> inds = weak ? lis_weak(v) : lis(v);
			for (int i = 0; i < ((int)(inds).size()-1); ++i) {
				assert(lt(v[inds[i]], v[inds[i+1]]));
			}
			for (int bi = 0; bi < ((1 << n)); ++bi) {
				int si = (int)bitset<32>(bi).count();
				if (si <= (int)(inds).size()) continue;
				int prev = INT_MIN;
				for (int i = 0; i < (n); ++i) if (bi & (1 << i)) {
					if (!lt(prev, v[i])) goto next;
					prev = v[i];
				}
				cout << "exists lis of size " << si << " but found only " << (int)(inds).size() << endl;
				for(auto &x: v) cout << x << ' ';
				cout << endl;
				abort();
	next:;
			}
		}
	}
	cout << "tests passed!" << endl;
}
