#include "../utilities/template.h"

#include "../../content/geometry/CirclePolygonIntersection.h"
#include "../utilities/genPolygon.h"

namespace orig{
typedef Point<long double> point_type;
long double area_ct(point_type pa, point_type pb, long double r) {
	if (pa.dist() < pb.dist()) swap(pa, pb);
	if (sgn(pb.dist()) == 0) return 0;
	long double a = pb.dist(), b = pa.dist(), c = (pb - pa).dist();
	long double sin_b = fabs(pb.cross(pb - pa) / a / c), cos_b = pb.dot(pb - pa) / a / c,
		sin_c = fabs(pa.cross(pb) / a / b), cos_c = pa.dot(pb) / a / b;
	long double B = atan2(sin_b, cos_b), C = atan2(sin_c, cos_c);
	if (a > r) {
		long double S = C / 2 * r * r, h = a * b * sin_c / c;
		if (h < r && B < M_PI / 2)
			S -= (acos(h / r) * r * r - h * sqrt(r * r - h * h));
		return S;
	} else if (b > r) {
		long double theta = M_PI - B - asin(sin_b / r * a);
		return a * r * sin(theta) / 2 + (C - theta) / 2 * r * r;
	} else return sin_c * a * b / 2;
}
long double circle_poly(point_type c, long double r, vector<point_type> poly) {
	long double area = 0;
	for (int i = 0; i < ((int)(poly).size()); ++i){
		auto a = poly[i] - c, b = poly[(i+1)%(int)(poly).size()] - c;
		area += area_ct(a, b, r) * sgn(a.cross(b));
	}
	return area;
}
}

signed main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	const int lim=5;
	for (int i=0;i<100000; i++) {

		vector<Point<int>> pts;
		for (int j=0; j<10; j++) {
			int x = rand()%lim, y = rand()%lim;
			pts.push_back(Point<int>(x, y));
		}

		auto poly_int = gen_polygon(pts);

		int cx = rand()%lim, cy = rand()%lim;
		auto c = point_type(cx, cy);
		auto c2 = orig::point_type(cx, cy);
		double r= rand()%(2*lim);

		vector<point_type> poly;
		vector<orig::point_type> poly2;
		for (auto j: poly_int) {
			poly.push_back(point_type(j.x, j.y));
			poly2.push_back(orig::point_type(j.x, j.y));
		}
		auto res1 = circle_poly(c, r, poly);
		auto res2 = orig::circle_poly(c2, r, poly2);

		if (abs(res1 - res2) > 1e-8) {
			cout<<abs(res1-res2)<<' '<<res1<<' '<<res2<<endl;
			assert(false);
		}
	}
	cout<<"tests passed!"<<endl;
}
