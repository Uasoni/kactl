/**
 * Author: stjepan glavina, chilli
 * Date: 2019-05-05
 * License: unlicense
 * Source: https://github.com/stjepang/snippets/blob/master/convex_hull.cpp
 * Description:
\\\begin{minipage}{75mm}
returns a vector of the points of the convex hull in counter-clockwise order.
points on the edge of the hull between two other points are not considered part of the hull.
\end{minipage}
\begin{minipage}{15mm}
\vspace{-6mm}
\includegraphics[width=\textwidth]{content/geometry/ConvexHull}
\vspace{-6mm}
\end{minipage}
 * Time: O(n \log n)
 * Status: stress-tested, tested with kattis:convexhull
*/
#pragma once

#include "Point.h"

template<class T>
vector<Point<T>> convex_hull(vector<Point<T>> pts) {
	if ((int)(pts).size() <= 1) return pts;
	sort(begin(pts), end(pts));
	vector<Point<T>> h((int)(pts).size()+1);
	int s = 0, t = 0;
	for (int it = 2; it--; s = --t, reverse(begin(pts), end(pts)))
		for (Point<T> p : pts) {
			while (t >= s + 2 && h[t-2].cross(h[t-1], p) <= 0) t--;
			h[t++] = p;
		}
	return {h.begin(), h.begin() + t - (t == 2 && h[0] == h[1])};
}
