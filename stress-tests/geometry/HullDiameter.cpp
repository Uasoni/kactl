#include "../utilities/template.h"

#include "../../content/geometry/ConvexHull.h"
#include "../../content/geometry/HullDiameter.h"

int main() {
	srand(2);
	for (int it = 0; it < (1000000); ++it) {
		int n = (rand() % 10) + 1;
		vector<Point<ll>> ps;
		for (int i = 0; i < (n); ++i) {
			ps.emplace_back(rand() % 11 - 5, rand() % 11 - 5);
		}
		ll r1 = 0;
		for (int i = 0; i < (n); ++i) for (int j = 0; j < (i); ++j) {
			r1 = max(r1, (ps[i] - ps[j]).dist2());
		}
		auto pa = hull_diameter(convex_hull(ps));
		ll r2 = ps.empty() ? 0LL : (pa[0] - pa[1]).dist2();
		assert(r1 == r2);
	}
	cout<<"tests passed!"<<endl;
}
