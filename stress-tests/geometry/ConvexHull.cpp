#include "../utilities/template.h"

#include "../../content/geometry/ConvexHull.h"
#include "../utilities/bench.h"
typedef Point<ll> point_type;

namespace old {
pair<vector<int>, vector<int>> ul_hull(const vector<point_type>& points) {
	vector<int> order((int)points.size()), upper, lower;
	iota(begin(order), end(order), 0);
	sort(begin(order), end(order), [&](int a, int b) { return points[a] < points[b]; });
	for (int index : order) {
		while (upper.size() > 1 && points[upper[upper.size() - 2]].cross(
			points[index], points[upper.back()]) <= 0) upper.pop_back();
		upper.push_back(index);
		while (lower.size() > 1 && points[lower[lower.size() - 2]].cross(
			points[index], points[lower.back()]) >= 0) lower.pop_back();
		lower.push_back(index);
	}
	return {upper, lower};
}

vector<int> convex_hull(const vector<point_type>& points) {
	vector<int> u, l; tie(u, l) = ul_hull(points);
	if (points.size() <= 1) return u;
	if (points[u[0]] == points[u[1]]) return {0};
	l.insert(l.end(), u.rbegin()+1, u.rend()-1);
	return l;
}
}

int main() {
	const int SIZE = 100;
	for (int t = 0; t < (100000); ++t) {
		const int GRID_SIZE = 1000;
		vector<point_type> pts(SIZE);
		for (int i = 0; i < SIZE; ++i)
			pts[i] = point_type(rand() % GRID_SIZE, rand() % GRID_SIZE);
		auto res = convex_hull(pts);
		auto res2 = old::convex_hull(pts);
		assert((int)(res).size() == (int)(res2).size());
		for (int i = 0; i < ((int)(res2).size()); ++i) {
			assert(pts[res2[i]] == res[i]);
		}
	}
	cout<<"tests passed!"<<endl;
}
