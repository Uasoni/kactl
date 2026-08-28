#include "../utilities/template.h"

#include "../../content/geometry/PolygonArea.h"
#include "../../content/geometry/PolygonCenter.h"
#include "../../content/geometry/InsidePolygon.h"

int main() {
	srand(0);
	typedef Point<double> point_type;
	vector<point_type> ps = {point_type{0,0}, point_type{6,4}, point_type{0,9}};
	int count = 0;
	point_type su{0,0};
	for (int it = 0; it < (100000); ++it) {
		double x = rand() / (RAND_MAX + 1.0);
		double y = rand() / (RAND_MAX + 1.0);
		x *= 10;
		y *= 10;
		if (!in_polygon(ps, point_type{x,y}, true)) continue;
		count++;
		su = su + point_type{x,y};
	}
	su = su / count;
	double approx_area = (double)count / 100000 * 100;
	assert(abs(polygon_area2(ps)/2.0 - approx_area) < 1);
	auto p = polygon_center(ps);
	assert(abs(p.x - su.x) < 1e-1 && abs(p.y - su.y) < 1e-1);
	cout<<"tests passed!"<<endl;
}
