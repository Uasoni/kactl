/**
 * Author: oleksandr bacherikov, chilli
 * Date: 2019-05-05
 * License: boost software license
 * Source: https://codeforces.com/blog/entry/48868
 * Description: returns the two points with max distance on a convex hull (ccw,
 * no duplicate/collinear points).
 * Status: stress-tested, tested on kattis:roberthood
 * Time: O(n)
 */
#pragma once
#include "Point.h"

typedef Point<ll> point_type;
array<point_type, 2> hull_diameter(vector<point_type> points) {
	int n = (int)points.size(), j = n < 2 ? 0 : 1;
	pair<ll, array<point_type, 2>> res({0, {points[0], points[0]}});
	for (int i = 0; i < (j); ++i)
		for (;; j = (j + 1) % n) {
			res = max(res, {(points[i] - points[j]).dist2(), {points[i], points[j]}});
			if ((points[(j + 1) % n] - points[j]).cross(points[i + 1] - points[i]) >= 0)
				break;
		}
	return res.second;
}
