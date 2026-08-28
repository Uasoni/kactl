#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;


const double EPS =1e-8;
#include "../utilities/genPolygon.h"
#include "../../content/geometry/InsidePolygon.h"
namespace old {

#include "../../content/geometry/OnSegment.h"
#include "../../content/geometry/SegmentDistance.h"

template<class it, class point_type>
bool inside_polygon(it begin, it end, const point_type& p,
		bool strict = true) {
	int n = 0; //number of isects with line from p to (inf,p.y)
	for (it i = begin, j = end-1; i != end; j = i++) {
		//if p is on edge of polygon
		if (on_segment(*i, *j, p)) return !strict;
		//or: if (seg_dist(*i, *j, p) <= epsilon) return !strict;
		//increment n if segment intersects line from p
		n += (max(i->y,j->y) > p.y && min(i->y,j->y) <= p.y &&
				((*j-*i).cross(p-*i) > 0) == (i->y <= p.y));
	}
	return n&1; //inside if odd number of intersections
}
}
typedef Point<double> point_type;
bool eq(point_type a, point_type b) {
	return (a-b).dist()<EPS;
}
const int NUMPOLY=100;
const int PTPERPOLY=100;
void test(int num_pts, int range) {
	for (int i = 0; i < (NUMPOLY); ++i) {
		vector<point_type> poly;
		for (int j = 0; j < (num_pts); ++j)
			poly.push_back(point_type(rand()%range, rand()%range));
		poly = gen_polygon(poly);
		for (int i = 0; i < (PTPERPOLY); ++i){
			point_type p(rand()%range, rand()%range);
			assert(in_polygon(poly, p, true) == old::inside_polygon(begin(poly), end(poly), p, true));
			assert(in_polygon(poly, p, false) == old::inside_polygon(begin(poly), end(poly), p, false));
		}
	}

}
int main() {
	test(20,5);
	test(1001,100);
	test(1000,1000);
	cout<<"tests passed!"<<endl;
}
