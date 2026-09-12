/**
 * Author: simon lindholm
 * Date: 2017-04-20
 * License: CC0
 * Source: own work
 * Description: container where you can add lines of the form kx+m, and query maximum values at points x.
 *  useful for dynamic programming (``convex hull trick'').
 * Integer slopes/intercepts/queries; insertion and query order are arbitrary.
 * Add at least one line before querying. For minima, add(-k,-m) and negate
 * query(x). No deletion. Products, differences and negations must fit in ll.
 * Usage: LineContainer hull;
 *  hull.add(2, 3); // y = 2*x + 3
 *  ll best = hull.query(4); // 11
 * Time: O(\log N)
 * Status: stress-tested
 */
#pragma once

struct Line {
	mutable ll k, m, p; // p: last integer x before next line wins
	bool operator<(const Line& o) const { return k < o.k; }
	bool operator<(ll x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
	// (for doubles, use INF = 1/.0, div(a,b) = a/b)
	static const ll INF = LLONG_MAX;
	ll div(ll a, ll b) { // floored division
		return a / b - ((a ^ b) < 0 && a % b); }
	bool isect(iterator x, iterator y) {
		if (y == end()) return x->p = INF, 0;
		if (x->k == y->k) x->p = x->m > y->m ? INF : -INF;
		else x->p = div(y->m - x->m, x->k - y->k);
		return x->p >= y->p;
	}
	void add(ll k, ll m) {
		auto z = insert({k, m, 0}), y = z++, x = y;
		while (isect(y, z)) z = erase(z);
		if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
		while ((y = x) != begin() && (--x)->p >= y->p)
			isect(x, erase(y));
	}
	ll query(ll x) {
		assert(!empty());
		auto l = *lower_bound(x);
		return l.k * x + l.m;
	}
};
