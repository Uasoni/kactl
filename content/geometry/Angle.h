/**
 * Author: simon lindholm
 * Date: 2015-01-31
 * License: CC0
 * Source: me
 * Description: A class for ordering angles (as represented by int points and
 *  a number of rotations around the origin). useful for rotational sweeping.
 *  sometimes also represents points or vectors.
 * Usage:
 *  vector<Angle> v = {w[0], w[0].t360() ...}; // sorted
 *  int j = 0; for (int i = 0; i < (n); ++i) { while (v[j] < v[i].t180()) ++j; }
 *  // sweeps j such that (j-i) represents the number of positively oriented triangles with vertices at 0 and i
 * Status: used, works well
 */
#pragma once

struct Angle {
	int x, y;
	int t;
	Angle(int x, int y, int t=0) : x(x), y(y), t(t) {}
	Angle operator-(Angle b) const { return {x-b.x, y-b.y, t}; }
	int half() const {
		assert(x || y);
		return y < 0 || (y == 0 && x < 0);
	}
	Angle t90() const { return {-y, x, t + (half() && x >= 0)}; }
	Angle t180() const { return {-x, -y, t + half()}; }
	Angle t360() const { return {x, y, t + 1}; }
};
bool operator<(Angle a, Angle b) {
	// add a.dist2() and b.dist2() to also compare distances
	return make_tuple(a.t, a.half(), a.y * (ll)b.x) <
	       make_tuple(b.t, b.half(), a.x * (ll)b.y);
}

// given two points, this calculates the smallest Angle between
// them, i.e., the Angle that covers the defined line segment.
pair<Angle, Angle> segment_angles(Angle a, Angle b) {
	if (b < a) swap(a, b);
	return (b < a.t180() ?
	        make_pair(a, b) : make_pair(b, a.t360()));
}
Angle operator+(Angle a, Angle b) { // Point a + vector b
	Angle r(a.x + b.x, a.y + b.y, a.t);
	if (a.t180() < r) r.t--;
	return r.t180() < a ? r.t360() : r;
}
Angle angle_diff(Angle a, Angle b) { // Angle b - Angle a
	int tu = b.t - a.t; a.t = b.t;
	return {a.x*b.x + a.y*b.y, a.x*b.y - a.y*b.x, tu - (b < a)};
}
