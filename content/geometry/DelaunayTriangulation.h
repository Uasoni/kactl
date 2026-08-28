/**
 * Author: mattias de zalenski
 * Date: unknown
 * Source: geometry in C
 * Description: computes the delaunay triangulation of a set of points.
 *  each circumcircle contains none of the input points.
 *  if any three points are collinear or any four are on the same circle,
 *  behavior is undefined. Callback point indices are 1-indexed.
 * Time: O(n^2)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"
#include "3dHull.h"

template<class P, class F>
void delaunay(vector<P>& ps, F trifun) {
	if ((int)(ps).size() == 3) { int d = (ps[0].cross(ps[1], ps[2]) < 0);
		trifun(1,2+d,3-d); }
	vector<Point3D<double>> p3;
	for (P p : ps) p3.emplace_back(p.x, p.y, p.dist2());
	if ((int)(ps).size() > 3) for(auto t:hull_3d(p3)) if ((p3[t.b-1]-p3[t.a-1]).
			cross(p3[t.c-1]-p3[t.a-1]).dot(Point3D<double>(0, 0, 1)) < 0)
		trifun(t.a, t.c, t.b);
}
