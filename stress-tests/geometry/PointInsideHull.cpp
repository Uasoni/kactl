#include "../utilities/template.h"

#include "../../content/geometry/ConvexHull.h"
namespace ignore {
	#include "../../content/geometry/SegmentDistance.h"
}
#include "../../content/geometry/PointInsideHull.h"
#include "../../content/geometry/InsidePolygon.h"
typedef Point<ll> point_type;

int main() {
	for (int it = 0; it < (100000); ++it) {
		int n = rand() % 15;
		vector<point_type> ps;
		for (int i = 0; i < (n); ++i) ps.emplace_back(rand() % 20 - 10, rand() % 20 - 10);
		vector<point_type> ps2 = convex_hull(ps);
		if (ps2.empty()) continue;
		for (int it2 = 0; it2 < (20); ++it2) {
			int x = rand() % 22 - 11;
			int y = rand() % 22 - 11;
			point_type p{x,y};
			assert(in_polygon(ps2, p, true) == (in_hull(ps2, p, true)));
			assert(in_polygon(ps2, p, false) == (in_hull(ps2, p, false)));
		}
	}
	cout<<"tests passed!"<<endl;
}
