#include "../utilities/template.h"

#include "../../content/geometry/Point.h"

template<>
struct Point<double> {
	typedef Point point_type;
	typedef double T;
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
	Point(const Point<ll>& other) : x((double)other.x), y((double)other.y) {}
	point_type& operator=(const Point<ll>& other) { x = (double)other.x; y = (double)other.y; return *this; }
	bool operator==(const Point<ll>& other) const { return *this == point_type(other); }
	bool operator<(point_type p) const { return x<p.x||(x==p.x && y<p.y);}
	bool operator==(point_type p) const { return x==p.x && y==p.y; }
	point_type operator+(point_type p) const { return point_type(x+p.x, y+p.y); }
	point_type operator-(point_type p) const { return point_type(x-p.x, y-p.y); }
	point_type operator*(T d) const { return point_type(x*d, y*d); }
	point_type operator/(T d) const { return point_type(x/d, y/d); }
	T dot(point_type p) const { return x*p.x + y*p.y; }
	T cross(point_type p) const { return x*p.y - y*p.x; }
	T cross(point_type a, point_type b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// Angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	point_type unit() const { return *this/dist(); } // makes dist()=1
	point_type perp() const { return point_type(-y, x); } // rotates +90 degrees
	point_type normal() const { return perp().unit(); }
	// returns Point rotated 'a' radians ccw around the origin
	point_type rotate(double a) const {
		return point_type(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
};

typedef Point<ll> point_type;

#include "../../content/geometry/ConvexHull.h"
#include "../../content/geometry/LineHullIntersection.h"

int segment_intersection(const point_type& s1, const point_type& e1,
		const point_type& s2, const point_type& e2, Point<double>& r1, Point<double>& r2) {
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
	typedef Point<double> PD;
	r1 = PD(s1) - PD(v1*a2)/(double)a;
	return 1;
}

int main() {
	srand(2);
	for (int it = 0; it < (1000000); ++it) {
		// cout<<endl;
		// cout<<"it: "<<it<<endl;
		int n = rand() % 15;
		vector<point_type> ps2;
		for (int i = 0; i < (n); ++i) ps2.emplace_back(rand() % 20 - 10, rand() % 20 - 10);
		vector<point_type> ps = convex_hull(ps2);
		if (ps.empty()) continue;
		point_type p{rand() % 20 - 10, rand() % 20 - 10};
		point_type q{rand() % 20 - 10, rand() % 20 - 10};

		n = (int)(ps).size();

		point_type delta = q - p, farp = p - delta * 50, farq = p + delta * 50;

		auto res = line_hull(p, q, ps);
		pii r = {res[0], res[1]};

		if (p == q) continue;

		auto fail = [&](int line) {
			cerr << (int)(ps).size() << endl;
			for(auto &p: ps) cout << p<<' ';
			cout<<endl;
			cout << "line: "<<p<<' '<<q<<endl;
			cout << "-> " << r.first << ' ' << r.second << endl;
			cout << "@line " << line << endl;
			abort();
		};
		auto require = [&](bool condition,
			const source_location& location = source_location::current()) {
			if (!condition) fail((int)location.line());
		};

		int any = 0, gen = 0, corner = -1, waspar = 0;
		vector<pair<Point<double>, int>> hits;
		for (int iter = 0; iter < (2); ++iter) for (int i = 0; i < (n); ++i) {
			Point<double> r1, r2;
			int j = (i+1) % n;
			int qu = segment_intersection(farp, farq, ps[i], ps[j], r1, r2);
			if (qu && (q - p).cross(ps[j] - ps[i]) == 0) { // parallel
				if (n != 2) {
					require(r.first == i || r.second == i);
				}
				// cerr << i << ' ' << j << ' ' << r.first << ' ' << r.second << endl;
				// assert(r.first == i && r.second == j);
				any = 1;
				if (iter == 0) gen = 10;
				waspar = 1;
			}
			else if (qu) {
				assert(qu != 2);
				if (r1 == ps[i]) {
					int k = (i-1+n) % n;
					require(r.first == i || r.second == i || r.first == k || r.second == k);
					if (iter == 1 && !waspar) require(r.first == i || r.second == i);
					if (iter == 0) corner = i;
					if (iter == 0) gen++;
					if (iter == 0) hits.emplace_back(r1, i);
				}
				else if (r1 == ps[j]) {
					require(r.first == i || r.second == i || r.first == j || r.second == j);
					if (iter == 1 && !waspar) require(r.first == j || r.second == j);
					if (iter == 0) corner = j;
					if (iter == 0) gen++;
				}
				else {
					require(r.first == i || r.second == i);
					if (iter == 0) gen = 10;
					if (iter == 0) hits.emplace_back(r1, i);
				}
				any = 1;
			}
		}

		if (!any) {
			assert(r.first == -1 && r.second == -1);
			continue;
		}
		if (!waspar) {
			require(r.first != r.second);
		}
		if (gen == 2) {
			assert(r.first == corner);
			require(r.second == -1);
		}
		if (n > 2 && ((int)(hits).size() == 1) != (r.second == -1 || r.first == r.second)) {
			cout<<"res: "<<r.first<<' '<<r.second<<endl;
			require(false);
		}
		assert((int)(hits).size() <= 2);
		if (r.first != r.second && (int)(hits).size() == 2) {
			assert(r.second != -1);
			assert(hits[0].second != hits[1].second);
			assert(hits[0].second == r.first || hits[0].second == r.second);
			assert(hits[1].second == r.first || hits[1].second == r.second);
			double dist0 = (hits[0].first - Point<double>(p)).dot(delta);
			double dist1 = (hits[1].first - Point<double>(p)).dot(delta);
			if (hits[0].second == r.first) {
				require(dist0 <= dist1);
			}
			else {
				require(dist0 >= dist1);
			}
		}

		res = line_hull(q, p, ps);
		pii R = {res[0], res[1]};
		if (r.second == -1) {
			assert(R == r);
		}
		else if (n == 2 && r.first == r.second) {
			assert(R.first == R.second);
		}
		else {
			assert(R.first == r.second);
			assert(R.second == r.first);
		}
	}
	cout<<"tests passed!"<<endl;
}
