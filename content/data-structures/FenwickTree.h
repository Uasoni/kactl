/**
 * Author: lukas polacek
 * Date: 2009-10-30
 * License: CC0
 * Source: folklore/top_coder
 * Description: computes 1-indexed inclusive prefix sums and updates single elements,
 * taking the difference between the old and new value.
 * Time: both operations are $O(\log N)$.
 * Status: stress-tested
 */
#pragma once

struct FenwickTree {
	vector<ll> s;
	FenwickTree(int n) : s(n + 1) {}
	void update(int pos, ll dif) { // a[pos] += dif, pos is 1-indexed
		for (; pos < (int)s.size(); pos += pos & -pos) s[pos] += dif;
	}
	ll query(int pos) { // sum of values in [1, pos]
		ll res = 0;
		for (; pos > 0; pos -= pos & -pos) res += s[pos];
		return res;
	}
	int lower_bound(ll sum) { // min pos such that sum [1, pos] >= sum
		// returns n + 1 if no prefix works, or 0 for the empty prefix.
		if (sum <= 0) return 0;
		int pos = 0;
		int pw = 1;
		while (pw < (int)s.size()) pw <<= 1;
		for (; pw; pw >>= 1) {
			if (pos + pw < (int)s.size() && s[pos + pw] < sum)
				pos += pw, sum -= s[pos];
		}
		return pos + 1;
	}
};
