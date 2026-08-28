#include "../utilities/template.h"

#include "../../content/geometry/PolygonArea.h"
#include "../../content/geometry/PolygonCut.h"
#include "../../content/geometry/sideOf.h"
#include "../../content/geometry/InsidePolygon.h"
#include "../../content/geometry/SegmentIntersection.h"

typedef Point<double> point_type;
int main() {
	for (int it = 0; it < (500); ++it) {
		int n = rand() % 10 + 3;
		vector<point_type> ps;
		for (int i = 0; i < (n); ++i) ps.emplace_back(rand() % 10 - 5, rand() % 10 - 5);
		point_type p(rand() % 10 - 5), q(rand() % 10 - 5);
		for (int i = 0; i < (n); ++i) for (int j = i+1; j < (n); ++j) {
			point_type a = ps[i], b = ps[(i+1)%n];
			point_type c = ps[j], d = ps[(j+1)%n];
			point_type r1, r2;
			auto r = seg_inter(a, b, c, d);
			if ((int)(r).size() == 2) goto fail;
			if ((int)(r).size() == 1) {
				if (i+1 == j || (j+1) % n == i) ;
				else goto fail;
			}
		}
		if (p == q) { fail: continue; }

		int count = 0;
		const int ITS = 400000;
		for (int it = 0; it < (ITS); ++it) {
			double x = rand() / (RAND_MAX + 1.0) * 10 - 5;
			double y = rand() / (RAND_MAX + 1.0) * 10 - 5;
			if (!in_polygon(ps, point_type{x,y}, true)) continue;
			if (side_of(p, q, point_type{x,y}) > 0) continue;
			count++;
		}
		double approx_area = (double)count / ITS * 100;

		ps = polygon_cut(ps, p, q);
		double real_area = ps.empty() ? 0.0 : abs(polygon_area2(ps) / 2.0);

		// cout << setprecision(2) << fixed;
		assert(real_area - approx_area < 2e-1);
		// cout << n << ' ' << real_area << '\t' << approx_area << '\t' << real_area - approx_area << endl;

		// cerr << n << endl;
		// for(auto &x: ps) {
			// cout << x.x << ' ' << x.y << endl;
		// }
	}
	cout<<"tests passed!"<<endl;
}
