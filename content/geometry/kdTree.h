/**
 * Author: stanford
 * Date: unknown
 * Source: stanford notebook
 * Description: KD-tree (2d, can be extended to 3d)
 * Status: tested on excellentengineers
 */
#pragma once

#include "Point.h"

typedef long long T;
typedef Point<T> point_type;
const T INF = numeric_limits<T>::max();

bool on_x(const point_type& a, const point_type& b) { return a.x < b.x; }
bool on_y(const point_type& a, const point_type& b) { return a.y < b.y; }

struct KdNode {
	point_type pt; // if this is a leaf, the single Point in it
	T x0 = INF, x1 = -INF, y0 = INF, y1 = -INF; // bounds
	KdNode *first = 0, *second = 0;

	T distance(const point_type& p) { // min squared distance to a Point
		T x = (p.x < x0 ? x0 : p.x > x1 ? x1 : p.x);
		T y = (p.y < y0 ? y0 : p.y > y1 ? y1 : p.y);
		return (point_type(x,y) - p).dist2();
	}

	KdNode(vector<point_type>&& vp) : pt(vp[0]) {
		for (point_type p : vp) {
			x0 = min(x0, p.x); x1 = max(x1, p.x);
			y0 = min(y0, p.y); y1 = max(y1, p.y);
		}
		if (vp.size() > 1) {
			// split on x if width >= height (not ideal...)
			sort(begin(vp), end(vp), x1 - x0 >= y1 - y0 ? on_x : on_y);
			// divide by taking half the array for each child (not
			// best performance with many duplicates in the middle)
			int half = (int)(vp).size()/2;
			first = new KdNode({vp.begin(), vp.begin() + half});
			second = new KdNode({vp.begin() + half, vp.end()});
		}
	}
};

struct KdTree {
	KdNode* root;
	KdTree(const vector<point_type>& vp) : root(new KdNode({begin(vp), end(vp)})) {}

	pair<T, point_type> search(KdNode *current, const point_type& p) {
		if (!current->first) {
			// uncomment if we should not find the Point itself:
			// if (p == node->pt) return {INF, point_type()};
			return make_pair((p - current->pt).dist2(), current->pt);
		}

		KdNode *f = current->first, *s = current->second;
		T bfirst = f->distance(p), bsec = s->distance(p);
		if (bfirst > bsec) swap(bsec, bfirst), swap(f, s);

		// search closest side first, other side if needed
		auto best = search(f, p);
		if (bsec < best.first)
			best = min(best, search(s, p));
		return best;
	}

	// find nearest Point to a Point, and its squared distance
	// (requires an arbitrary operator< for Point)
	pair<T, point_type> nearest(const point_type& p) {
		return search(root, p);
	}
};
