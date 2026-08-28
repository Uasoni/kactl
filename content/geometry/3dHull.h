/**
 * Author: johan sannemo
 * Date: 2017-04-18
 * Source: derived from https://gist.github.com/msg555/4963794 by mark gordon
 * Description: computes all faces of the 3-dimension hull of a Point set.
 *  *no four points must be coplanar*, or else random results will be returned.
 *  all faces will Point outwards. Face vertex indices are 1-indexed.
 * Time: O(n^2)
 * Status: tested on SPOJ CH3D
 */
#pragma once

#include "Point3D.h"

typedef Point3D<double> hull_point_3d;

struct EdgeFaces {
	void ins(int x) { (a == -1 ? a : b) = x; }
	void rem(int x) { (a == x ? a : b) = -1; }
	int cnt() { return (a != -1) + (b != -1); }
	int a, b;
};

struct Face { hull_point_3d normal; int a, b, c; };

vector<Face> hull_3d(const vector<hull_point_3d>& points) {
	assert((int)points.size() >= 4);
	vector<vector<EdgeFaces>> edges((int)points.size(),
		vector<EdgeFaces>((int)points.size(), {-1, -1}));
	vector<Face> faces;
	auto mf = [&](int i, int j, int k, int l) {
		hull_point_3d q = (points[j] - points[i]).cross(points[k] - points[i]);
		if (q.dot(points[l]) > q.dot(points[i]))
			q = q * -1;
		Face current{q, i, j, k};
		edges[current.a][current.b].ins(k);
		edges[current.a][current.c].ins(j);
		edges[current.b][current.c].ins(i);
		faces.push_back(current);
	};
	for (int i = 0; i < (4); ++i) for (int j = i+1; j < (4); ++j) for (int k = j+1; k < (4); ++k)
		mf(i, j, k, 6 - i - j - k);

	for (int i = 4; i < (int)points.size(); ++i) {
		for (int j = 0; j < (int)faces.size(); ++j) {
			Face current = faces[j];
			if(current.normal.dot(points[i])
					> current.normal.dot(points[current.a])) {
				edges[current.a][current.b].rem(current.c);
				edges[current.a][current.c].rem(current.b);
				edges[current.b][current.c].rem(current.a);
				swap(faces[j--], faces.back());
				faces.pop_back();
			}
		}
		int nw = (int)faces.size();
		for (int j = 0; j < (nw); ++j) {
			Face current = faces[j];
			if (edges[current.a][current.b].cnt() != 2)
				mf(current.a, current.b, i, current.c);
			if (edges[current.a][current.c].cnt() != 2)
				mf(current.a, current.c, i, current.b);
			if (edges[current.b][current.c].cnt() != 2)
				mf(current.b, current.c, i, current.a);
		}
	}
	for (Face& current : faces) if ((points[current.b] - points[current.a]).cross(
		points[current.c] - points[current.a]).dot(current.normal) <= 0)
		swap(current.c, current.b);
	for (Face& face : faces) ++face.a, ++face.b, ++face.c;
	return faces;
};
