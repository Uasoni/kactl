/**
 * Author: oleksandr bacherikov, chilli
 * Date: 2019-05-07
 * License: boost software license
 * Source: https://github.com/al_cash07/ACTL/blob/master/include/actl/geometry/algorithm/intersect/line_convex_polygon.hpp
 * Description: line-convex polygon intersection. the polygon must be ccw and have no collinear points.
 * line\_hull(line, poly) returns a pair describing the intersection of a line with the polygon:
 *  \begin{itemize*}
 *    \item $(0, 0)$ if no collision,
 *    \item $(i, 0)$ if touching the corner $i$,
 *    \item $(i, i)$ if along side $(i, i+1)$,
 *    \item $(i, j)$ if crossing sides $(i, i+1)$ and $(j, j+1)$.
 *  \end{itemize*}
 *  in the last case, if a corner $i$ is crossed, this is treated as happening on side $(i, i+1)$.
 *  the points are returned in the same order as the line hits the polygon.
 * Polygon vertex and side indices are 1-indexed. \texttt{extreme\_vertex}
 * returns the index of a hull vertex with maximum projection onto a line.
 * Time: O(\log n)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"

template <class P> int extreme_vertex_index(vector<P>& poly, P dir) {
	int n = (int)(poly).size(), lo = 0, hi = n;
	auto compare = [&](int i, int j) {
		return sgn(dir.perp().cross(poly[i % n] - poly[j % n]));
	};
	auto is_extreme = [&](int i) {
		return compare(i + 1, i) >= 0
			&& compare(i, i - 1 + n) < 0;
	};
	if (is_extreme(0)) return 0;
	while (lo + 1 < hi) {
		int m = (lo + hi) / 2;
		if (is_extreme(m)) return m;
		int ls = compare(lo + 1, lo), ms = compare(m + 1, m);
		(ls < ms || (ls == ms && ls == compare(lo, m)) ? hi : lo) = m;
	}
	return lo;
}

template <class P> int extreme_vertex(vector<P>& poly, P dir) {
	return extreme_vertex_index(poly, dir) + 1;
}

template <class P>
array<int, 2> line_hull(P a, P b, vector<P>& poly) {
	auto compare_line = [&](int i) { return sgn(a.cross(poly[i], b)); };
	int end_a = extreme_vertex_index(poly, (a - b).perp());
	int end_b = extreme_vertex_index(poly, (b - a).perp());
	if (compare_line(end_a) < 0 || compare_line(end_b) > 0)
		return {0, 0};
	array<int, 2> res;
	for (int i = 0; i < (2); ++i) {
		int lo = end_b, hi = end_a, n = (int)(poly).size();
		while ((lo + 1) % n != hi) {
			int m = ((lo + hi + (lo < hi ? 0 : n)) / 2) % n;
			(compare_line(m) == compare_line(end_b) ? lo : hi) = m;
		}
		res[i] = (lo + !compare_line(hi)) % n;
		swap(end_a, end_b);
	}
	if (res[0] == res[1]) return {res[0] + 1, 0};
	if (!compare_line(res[0]) && !compare_line(res[1]))
		switch ((res[0] - res[1] + (int)(poly).size() + 1) % (int)(poly).size()) {
			case 0: return {res[0] + 1, res[0] + 1};
			case 2: return {res[1] + 1, res[1] + 1};
		}
	return {res[0] + 1, res[1] + 1};
}
