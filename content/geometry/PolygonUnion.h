/**
 * Author: black_horse2014, chilli
 * Date: 2019-10-29
 * License: unknown
 * Source: https://codeforces.com/gym/101673/submission/50481926
 * Description: calculates the area of the union of $n$ polygons (not necessarily
 * convex). the points within each polygon must be given in CCW order.
 * (epsilon checks may optionally be added to side\_of/sgn, but shouldn't be needed.)
 * Time: $O(N^2)$, where $N$ is the total number of points
 * Status: stress-tested, submitted on ECNA 2017 problem A
 */
#pragma once

#include "Point.h"
#include "sideOf.h"

typedef Point<double> point_type;
double rat(point_type a, point_type b) { return sgn(b.x) ? a.x/b.x : a.y/b.y; }
double poly_union(vector<vector<point_type>>& poly) {
	double ret = 0;
	for (int i = 0; i < ((int)(poly).size()); ++i) for (int v = 0; v < ((int)(poly[i]).size()); ++v) {
		point_type a = poly[i][v], b = poly[i][(v + 1) % (int)(poly[i]).size()];
		vector<pair<double, int>> segs = {{0, 0}, {1, 0}};
		for (int j = 0; j < ((int)(poly).size()); ++j) if (i != j) {
			for (int u = 0; u < ((int)(poly[j]).size()); ++u) {
				point_type c = poly[j][u], d = poly[j][(u + 1) % (int)(poly[j]).size()];
				int sc = side_of(a, b, c), sd = side_of(a, b, d);
				if (sc != sd) {
					double sa = c.cross(d, a), sb = c.cross(d, b);
					if (min(sc, sd) < 0)
						segs.emplace_back(sa / (sa - sb), sgn(sc - sd));
				} else if (!sc && !sd && j<i && sgn((b-a).dot(d-c))>0){
					segs.emplace_back(rat(c - a, b - a), 1);
					segs.emplace_back(rat(d - a, b - a), -1);
				}
			}
		}
		sort(begin(segs), end(segs));
		for (auto& s : segs) s.first = min(max(s.first, 0.0), 1.0);
		double sum = 0;
		int cnt = segs[0].second;
		for (int j = 1; j < ((int)(segs).size()); ++j) {
			if (!cnt) sum += segs[j].first - segs[j - 1].first;
			cnt += segs[j].second;
		}
		ret += a.cross(b) * sum;
	}
	return ret / 2;
}
