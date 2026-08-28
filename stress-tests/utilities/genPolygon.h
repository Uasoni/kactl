#pragma once

// translated from python code posted here: https://codeforces.com/blog/entry/63058?#comment-472788
// may generate polygons with colinear points. won't generate polygons with duplicate points. no guarantee of the direction of the polygon.
#include "../../content/geometry/Point.h"
#include "../../content/geometry/PolygonArea.h"
#include "random.h"

template<class P> pair<bool, vector<P>> conquer(vector<P> pts, int depth) {
	if (depth>100) {
		return {false, {}};
	}
	if ((int)(pts).size() <= 2) return {true,pts};
	if ((int)(pts).size() == 3) {
		swap(pts[1], pts[2]);
		return {true,pts};
	}

	int divide_id = rand_range(2, (int)(pts).size());
	P p1 = pts[divide_id];
	double divide_k = rand_double(0.01, 0.99);
	P p2(divide_k*(pts[1].x-pts[0].x) + pts[0].x, divide_k*(pts[1].y - pts[0].y) + pts[0].y);
	vector<double> line = {p2.y - p1.y, p1.x - p2.x, -p1.x*p2.y + p1.y*p2.x};
	int idx0 = ((line[0]*pts[0].x + line[1]*pts[0].y + line[2]) >=0);
	int idx1 = ((line[0]*pts[1].x + line[1]*pts[1].y + line[2]) >=0);
	if (idx0==idx1)
		return conquer(pts, depth+1);
	array<vector<P>, 2> sides;
	sides[idx0].push_back(pts[0]);
	sides[idx0].push_back(p1);
	sides[!idx0].push_back(p1);
	sides[!idx0].push_back(pts[1]);
	for (int i = 2; i < ((int)(pts).size()); ++i) {
		if (i == divide_id) continue;
		int idx = ((line[0]*pts[i].x + line[1]*pts[i].y + line[2]) >=0);
		sides[idx].push_back(pts[i]);
	}
	auto pa = conquer(sides[idx0], depth+1);
	auto pb = conquer(sides[!idx0], depth+1);
	if (!pa.first || !pb.first) return {false, {}};
	pb.second.erase(pb.second.begin());
	pa.second.insert(pa.second.end(), begin(pb.second), end(pb.second));
	return pa;
}
template<class P> vector<P> gen_polygon(vector<P> pts, int depth=0) {
	if (depth>100) return {P(0,0), P(1,0), P(0,1)};
	sort(begin(pts), end(pts));
	pts.resize(unique(begin(pts), end(pts)) - pts.begin());
	shuffle_vec(pts);
	if ((int)(pts).size() <=3) return pts;
	vector<double> line ={(double)(pts[1].y-pts[0].y), (double)(pts[0].x - pts[1].x), (double)(-pts[0].x*pts[1].y + pts[0].y*pts[1].x)};
	array<vector<P>, 2> sides;
	sides[0].push_back(pts[0]);
	sides[0].push_back(pts[1]);
	sides[1].push_back(pts[1]);
	sides[1].push_back(pts[0]);
	for (int i = 2; i < ((int)(pts).size()); ++i) {
		int idx = (line[0]*pts[i].x + line[1]*pts[i].y + line[2]) >=0;
		sides[idx].push_back(pts[i]);
	}
	auto ta = conquer(sides[0],0);
	auto tb = conquer(sides[1],0);
	auto pa=ta.second, pb=tb.second;
	if (!ta.first || !tb.first) {
		return gen_polygon(pts, depth+1);
	}
	pa.erase(pa.begin());
	pb.erase(pb.begin());
	pa.insert(pa.end(), begin(pb), end(pb));
	if (polygon_area2(pa) < 0) reverse(begin(pa), end(pa));
	return pa;
}
