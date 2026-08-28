/**
 * Author: simon lindholm
 * License: CC0
 * Source: codeforces
 * Description: given $a[i] = \min_{lo(i) \le k \le hi(i)}(f(i, k))$ where the (minimal)
 * optimal $k$ increases with $i$, computes $a[i]$ for the inclusive range $[L,R]$.
 * Time: O((N + (hi-lo)) \log N)
 * Status: tested on http://codeforces.com/contest/321/problem/E
 */
#pragma once

struct DpState { // modify at will:
	int lo(int ind) { return 1; }
	int hi(int ind) { return ind; }
	ll f(int ind, int k) { return dp[ind][k]; }
	void store(int ind, int k, ll v) { res[ind] = pii(k, v); }

	void rec(int left, int right, int opt_left, int opt_right) {
		if (left > right) return;
		int mid = (left + right) >> 1;
		pair<ll, int> best(LLONG_MAX, opt_left);
		for (int k = max(opt_left, lo(mid)); k <= min(opt_right, hi(mid)); ++k)
			best = min(best, make_pair(f(mid, k), k));
		store(mid, best.second, best.first);
		rec(left, mid - 1, opt_left, best.second);
		rec(mid + 1, right, best.second, opt_right);
	}
	void solve(int left, int right) { rec(left, right, INT_MIN, INT_MAX); }
};
