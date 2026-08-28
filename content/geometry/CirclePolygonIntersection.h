/**
 * Author: chilli, takanori MAEHARA
 * Date: 2019-10-31
 * License: CC0
 * Source: https://github.com/spaghetti-source/algorithm/blob/master/geometry/_geom.cc#L744
 * Description: returns the area of the intersection of a circle with a
 * ccw polygon.
 * Time: O(n)
 * Status: tested on GNYR 2019 gerrymandering, stress-tested
 */
#pragma once

#include "../../content/geometry/Point.h"

typedef Point<double> point_type;
double circle_poly(point_type c, double r, vector<point_type> ps) {
	auto angle_between = [](point_type p, point_type q) {
		return atan2(p.cross(q), p.dot(q));
	};
	auto tri = [&](point_type p, point_type q) {
		auto r2 = r * r / 2;
		point_type d = q - p;
		auto a = d.dot(p)/d.dist2(), b = (p.dist2()-r*r)/d.dist2();
		auto det = a * a - b;
		if (det <= 0) return angle_between(p, q) * r2;
		auto s = max(0., -a-sqrt(det)), t = min(1., -a+sqrt(det));
		if (t < 0 || 1 <= s) return angle_between(p, q) * r2;
		point_type u = p + d * s, v = q + d * (t-1);
		return angle_between(p, u) * r2 + u.cross(v) / 2
			+ angle_between(v, q) * r2;
	};
	auto sum = 0.0;
	for (int i = 0; i < ((int)(ps).size()); ++i)
		sum += tri(ps[i] - c, ps[(i + 1) % (int)(ps).size()] - c);
	return sum;
}
