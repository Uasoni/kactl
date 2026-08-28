/**
 * Author: chilli
 * Date: 2019-05-17
 * License: CC0
 * Source: https://github.com/ngthanhtrung23/acm_notebook_new
 * Description: determine whether a Point t lies inside a convex hull (CCW
 * order, with no collinear points). returns true if Point lies within
 * the hull. if strict is true, points on the boundary aren't included.
 * Usage:
 * Status: stress-tested
 * Time: O(\log N)
 */
#pragma once

#include "Point.h"
#include "sideOf.h"
#include "OnSegment.h"

typedef Point<ll> point_type;

bool in_hull(const vector<point_type>& l, point_type p, bool strict = true) {
	int a = 1, b = (int)(l).size() - 1, r = !strict;
	if ((int)(l).size() < 3) return r && on_segment(l[0], l.back(), p);
	if (side_of(l[0], l[a], l[b]) > 0) swap(a, b);
	if (side_of(l[0], l[a], p) >= r || side_of(l[0], l[b], p)<= -r)
		return false;
	while (abs(a - b) > 1) {
		int c = (a + b) / 2;
		(side_of(l[0], l[c], p) > 0 ? b : a) = c;
	}
	return sgn(l[a].cross(l[b], p)) < r;
}
