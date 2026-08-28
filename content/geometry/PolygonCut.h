/**
 * Author: ulf lundstrom
 * Date: 2009-03-21
 * License: CC0
 * Source:
 * Description:\\
\begin{minipage}{75mm}
 returns a vector with the vertices of a polygon with everything to the left of the line going from s to e cut away.
\end{minipage}
\begin{minipage}{15mm}
\vspace{-6mm}
\includegraphics[width=\textwidth]{content/geometry/PolygonCut}
\vspace{-6mm}
\end{minipage}
 * Usage:
 * 	vector<point_type> p = ...;
 * 	p = polygon_cut(p, point_type(0,0), point_type(1,0));
 * Status: tested but not extensively
 */
#pragma once

#include "Point.h"

typedef Point<double> point_type;
vector<point_type> polygon_cut(const vector<point_type>& poly, point_type s, point_type e) {
	vector<point_type> res;
	for (int i = 0; i < ((int)(poly).size()); ++i) {
		point_type cur = poly[i], prev = i ? poly[i-1] : poly.back();
		auto a = s.cross(e, cur), b = s.cross(e, prev);
		if ((a < 0) != (b < 0))
			res.push_back(cur + (prev - cur) * (a / (a - b)));
		if (a < 0)
			res.push_back(cur);
	}
	return res;
}
