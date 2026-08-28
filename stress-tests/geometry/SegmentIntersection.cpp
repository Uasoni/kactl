#include "../utilities/template.h"

#include "../../content/geometry/SegmentIntersection.h"

namespace old_impl {
template<class point_type>
int segment_intersection(const point_type& s1, const point_type& e1,
		const point_type& s2, const point_type& e2, point_type& r1, point_type& r2) {
	if (e1==s1) {
		if (e2==s2) {
			if (e1==e2) { r1 = e1; return 1; } //all equal
			else return 0; //different Point segments
		} else return segment_intersection(s2,e2,s1,e1,r1,r2);//swap
	}
	//segment directions and separation
	point_type v1 = e1-s1, v2 = e2-s2, d = s2-s1;
	auto a = v1.cross(v2), a1 = v1.cross(d), a2 = v2.cross(d);
	if (a == 0) { //if parallel
		auto b1=s1.dot(v1), c1=e1.dot(v1),
			 b2=s2.dot(v1), c2=e2.dot(v1);
		if (a1 || a2 || max(b1,min(b2,c2))>min(c1,max(b2,c2)))
			return 0;
		r1 = min(b2,c2)<b1 ? s1 : (b2<c2 ? s2 : e2);
		r2 = max(b2,c2)>c1 ? e1 : (b2>c2 ? s2 : e2);
		return 2-(r1==r2);
	}
	if (a < 0) { a = -a; a1 = -a1; a2 = -a2; }
	if (0<a1 || a<-a1 || 0<a2 || a<-a2)
		return 0;
	r1 = s1-v1*a2/a;
	return 1;
}
}
typedef Point<double> point_type;
bool eq(point_type a, point_type b) {
	return (a-b).dist()<1e-8;
}
int main() {
	for (int t = 0; t < (1000000); ++t) {
		const int GRID=6;
		point_type a(rand()%GRID, rand()%GRID), b(rand()%GRID, rand()%GRID), c(rand()%GRID, rand()%GRID), d(rand()%GRID, rand()%GRID);
		point_type tmp1, tmp2;
		auto res = old_impl::segment_intersection(a,b,c,d, tmp1, tmp2);
		auto res2 = seg_inter(a,b,c,d);
		if (res != (int)(res2).size()) {
			cout<<a<<' '<<b<<' '<<c<<' '<<d<<endl;
			cout<<"old: "<<res<<" new: "<<(int)(res2).size()<<endl;
		}
		assert(res==(int)(res2).size());
		if (res==1) {
			assert(eq(*res2.begin(), tmp1));
		} else if (res==2) {
			vector<point_type> a(res2.begin(), res2.end());
			vector<point_type> b({tmp1, tmp2});
			sort(begin(b), end(b));
			assert(eq(a[0], b[0]) && eq(a[1],b[1]));
		}
	}
	cout<<"tests passed!"<<endl;
}
