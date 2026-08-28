/**
 * Author: chilli, takanori MAEHARA
 * Date: 2019-11-02
 * License: CC0
 * Source: https://github.com/spaghetti-source/algorithm/blob/master/geometry/rectilinear_mst.cc
 * Description: given N points, returns up to 4*N edges, which are guaranteed
 * to contain a minimum spanning tree for the graph with edge weights w(p, q) =
 * |p.x - q.x| + |p.y - q.y|. edges are in the form (distance, src, dst). use a
 * standard MST algorithm on the result to find the final MST. Source and
 * destination indices are 1-indexed.
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once
#include "Point.h"

typedef Point<int> point_type;
vector<array<int, 3>> manhattan_mst(vector<point_type> ps) {
	vector<int> id((int)(ps).size());
	iota(begin(id), end(id), 0);
	vector<array<int, 3>> edges;
	for (int k = 0; k < (4); ++k) {
		sort(begin(id), end(id), [&](int i, int j) {
		     return (ps[i]-ps[j]).x < (ps[j]-ps[i]).y;});
		map<int, int> sweep;
		for (int i : id) {
			for (auto it = sweep.lower_bound(-ps[i].y);
				        it != sweep.end(); sweep.erase(it++)) {
				int j = it->second;
				point_type d = ps[i] - ps[j];
				if (d.y > d.x) break;
				edges.push_back({d.y + d.x, i + 1, j + 1});
			}
			sweep[-ps[i].y] = i;
		}
		for (point_type& p : ps) if (k & 1) p.x = -p.x; else swap(p.x, p.y);
	}
	return edges;
}
