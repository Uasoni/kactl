/**
 * Author: ulf lundstrom
 * Date: 2009-03-21
 * License: CC0
 * Source: tiny_kactl
 * Description: returns twice the signed area of a polygon.
 *  clockwise enumeration gives negative area. watch out for overflow if using int as T!
 * Status: stress-tested and tested on kattis:polygonarea
 */
#pragma once

#include "Point.h"

template<class T>
T polygon_area2(vector<Point<T>>& v) {
	T a = v.back().cross(v[0]);
	for (int i = 0; i < ((int)(v).size()-1); ++i) a += v[i].cross(v[i+1]);
	return a;
}
