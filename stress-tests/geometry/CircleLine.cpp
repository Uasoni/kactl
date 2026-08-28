#include "../utilities/template.h"
#include "../utilities/randGeo.h"

#include "../../content/geometry/lineDistance.h"
#include "../../content/geometry/CircleLine.h"

typedef Point<double> point_type;
int main() {
	{
		auto res = circle_line(point_type(0, 0), 1, point_type(-1, -1), point_type(1, 1));
		assert(res.size() == 2);
		assert((res[1]-point_type(sqrt(2)/2, sqrt(2)/2)).dist() < 1e-8);
	}
	{
		auto res = circle_line(point_type(0, 0), 1, point_type(-5,  1), point_type(5, 1));
		assert(res.size() == 1);
		assert((res[0]-point_type(0,1)).dist() < 1e-8);
	}
	{
		auto res = circle_line(point_type(4, 4), 1, point_type(0,  0), point_type(5, 0));
		assert(res.size() == 0);
	}
	for (int it = 0; it < (100000); ++it) {
		point_type a = rand_int_pt(5);
		point_type b = rand_int_pt(5);
		point_type c = rand_int_pt(5);
		if (a == b) {
			// not a well defined line
			continue;
		}
		double r = sqrt(rand() % 49);
		vector<point_type> points = circle_line(c, r, a, b);

		// soundness
		assert((int)(points).size() <= 2);
		for (point_type p : points) {
			// Point is on circle
			assert(abs((p - c).dist() - r) < 1e-6);
			// Point is on line
			assert(line_dist(a, b, p) < 1e-6);
		}

		// best-effort completeness check:
		// in some easy cases we must have points in the intersection.
		if ((a - c).dist() < r - 1e-6 || (b - c).dist() < r - 1e-6 || ((a + b) / 2 - c).dist() < r - 1e-6) {
			assert(!points.empty());
		}
	}
	cout<<"tests passed!"<<endl;
}
