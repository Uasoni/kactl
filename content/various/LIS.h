/**
 * Author: johan sannemo
 * License: CC0
 * Description: compute 1-indexed positions of a longest increasing subsequence.
 * Time: $O(N \log N)$
 * Status: tested on kattis:longincsubseq, stress-tested
 */
#pragma once

template<class I> vector<int> lis(const vector<I>& values) {
	if (values.empty()) return {};
	vector<int> prev((int)(values).size(), -1);
	vector<pair<I, int>> res;
	for (int i = 0; i < ((int)(values).size()); ++i) {
		// change 0 -> i for longest non-decreasing subsequence
		auto it = lower_bound(begin(res), end(res), pair<I, int>{values[i], 0});
		if (it == res.end()) res.emplace_back(), it = res.end()-1;
		*it = {values[i], i};
		prev[i] = it == res.begin() ? -1 : (it-1)->second;
	}
	int length = (int)res.size(), cur = res.back().second;
	vector<int> ans(length);
	while (length--) ans[length] = cur + 1, cur = prev[cur];
	return ans;
}
