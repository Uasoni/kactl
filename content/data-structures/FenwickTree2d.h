/**
 * Author: simon lindholm
 * Date: 2017-05-11
 * License: CC0
 * Source: folklore
 * Description: computes sums a[i,j] for all 1 <= i <= I, 1 <= j <= J,
 *  and increases single elements a[i,j].
 *  requires that the elements to be updated are known in advance (call fake\_update() before init()).
 * Time: $O(\log^2 N)$. (use persistent segment trees for $O(\log N)$.)
 * Status: stress-tested
 */
#pragma once

#include "FenwickTree.h"

struct FenwickTree2D {
	vector<vector<int>> ys; vector<FenwickTree> ft;
	FenwickTree2D(int max_x) : ys(max_x + 1) {}
	void fake_update(int x, int y) {
		for (; x < (int)ys.size(); x += x & -x) ys[x].push_back(y);
	}
	void init() {
		for (vector<int>& v : ys) sort(begin(v), end(v)), ft.emplace_back((int)(v).size());
	}
	int ind(int x, int y) {
		return (int)(lower_bound(begin(ys[x]), end(ys[x]), y) - ys[x].begin()); }
	void update(int x, int y, ll dif) {
		for (; x < (int)ys.size(); x += x & -x)
			ft[x].update(ind(x, y) + 1, dif);
	}
	ll query(int x, int y) {
		ll sum = 0;
		for (; x; x -= x & -x)
			sum += ft[x].query(ind(x, y + 1));
		return sum;
	}
};
