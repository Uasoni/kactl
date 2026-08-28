/**
 * Author: simon lindholm
 * Date: 2019-04-17
 * License: CC0
 * Source: https://codeforces.com/blog/entry/58747
 * Description: finds the closest pair of points.
 * Time: O(n \log n)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"

typedef Point<ll> point_type;
pair<point_type, point_type> closest(vector<point_type> v) {
	assert((int)(v).size() > 1);
	set<point_type> candidates;
	sort(begin(v), end(v), [](point_type a, point_type b) { return a.y < b.y; });
	pair<ll, pair<point_type, point_type>> ret{LLONG_MAX, {point_type(), point_type()}};
	int j = 0;
	for (point_type p : v) {
		point_type d{1 + (ll)sqrt(ret.first), 0};
		while (v[j].y <= p.y - d.x) candidates.erase(v[j++]);
		auto lo = candidates.lower_bound(p - d), hi = candidates.upper_bound(p + d);
		for (; lo != hi; ++lo)
			ret = min(ret, {(*lo - p).dist2(), {*lo, p}});
		candidates.insert(p);
	}
	return ret.second;
}
