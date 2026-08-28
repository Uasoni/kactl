/**
 * Author: ulf lundstrom
 * Date: 2009-04-08
 * License: CC0
 * Source:
 * Description: returns the center of mass for a polygon.
 * Time: O(n)
 * Status: tested
 */
#pragma once

#include "Point.h"

typedef Point<double> point_type;
point_type polygon_center(const vector<point_type>& v) {
	point_type res(0, 0); double area2 = 0;
	for (int i = 0, j = (int)(v).size() - 1; i < (int)(v).size(); j = i++) {
		res = res + (v[i] + v[j]) * v[j].cross(v[i]);
		area2 += v[j].cross(v[i]);
	}
	return res / area2 / 3;
}
