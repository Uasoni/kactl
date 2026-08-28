#include "../utilities/template.h"

// #define TEST_PERF

#include "../../content/geometry/ConvexHull.h"
#include "../../content/geometry/PolygonArea.h"

#include "../../content/geometry/circumcircle.h"
typedef Point<ll> integer_point;
typedef Point<double> floating_point_type;

floating_point_type top(integer_point x) {
	return floating_point_type((double)x.x, (double)x.y);
}

struct BumpAllocator {
	char buf[450 << 20];
	size_t bufp;
	void* alloc(size_t s) {
		assert(s < bufp);
		return (void*)&buf[bufp -= s];
	}
	BumpAllocator() { reset(); }

	template<class T> T* operator=(T&& x) {
		T* r = (T*)alloc(sizeof(T));
		new(r) T(move(x));
		return r;
	}
	void reset() { bufp = sizeof buf; }
} BumpAllocator;

// when not testing perf, we don't want to leak memory
#ifndef TEST_PERF
#define new BumpAllocator =
#endif
#include "../../content/geometry/FastDelaunay.h"
#ifndef TEST_PERF
#undef new
#endif

template<class A, class F>
void dela(A& v, F f) {
	auto ret = triangulate(v);
	assert((int)(ret).size() % 3 == 0);
	map<integer_point, int> lut;
	for (int i = 0; i < ((int)(v).size()); ++i) lut[v[i]] = i;
	for (int a = 0; a < (int)(ret).size(); a += 3) {
		f(lut[ret[a]], lut[ret[a+1]], lut[ret[a+2]]);
	}
}

int main1() {
	srand(2);
	feenableexcept(29);
	for (int it = 0; it < (3000000); ++it) {{
		BumpAllocator.reset();
		// if (it % 200 == 0) cerr << endl;
		vector<integer_point> ps;
		int n = rand() % 20 + 1;
		int xrange = rand() % 50 + 1;
		int yrange = rand() % 50 + 1;
		for (int i = 0; i < (n); ++i) {
			ps.emplace_back(rand() % (2*xrange) - xrange, rand() % (2*yrange) - yrange);
		}

		auto coc = [&](int i, int j, int k, int l) {
			double a = (ps[i] - ps[j]).dist();
			double b = (ps[j] - ps[k]).dist();
			double c = (ps[k] - ps[l]).dist();
			double d = (ps[l] - ps[i]).dist();
			double e = (ps[i] - ps[k]).dist();
			double f = (ps[j] - ps[l]).dist();
			double q = a*c + b*d - e*f;
			return abs(q) < 1e-4;
		};

		for (int i = 0; i < (n); ++i) for (int j = 0; j < (i); ++j) {
			// identical
			if (ps[i] == ps[j]) {  goto fail; }
		}
		if (false) for (int i = 0; i < (n); ++i) for (int j = 0; j < (i); ++j) for (int k = 0; k < (j); ++k) {
			// colinear
			if (ps[i].cross(ps[j], ps[k]) == 0) {  goto fail; }
		}
		if (false) for (int i = 0; i < (n); ++i) for (int j = 0; j < (i); ++j) for (int k = 0; k < (j); ++k) for (int l = 0; l < (k); ++l) {
			// concyclic
			if (coc(i,j,k,l) || coc(i,j,l,k) || coc(i,l,j,k) || coc(i,l,k,j)) {  goto fail; }
		}

		bool all_colinear = true;
		if (n >= 3) {
			for (int i = 2; i < (n); ++i) if ((ps[i] - ps[0]).cross(ps[1] - ps[0])) all_colinear = false;
		}

		auto fail = [&]() {
			cout << "points:" << endl;
			for(auto &p: ps) {
				cout << p.x << ' ' << p.y << endl;
			}

			cout << "triangles:" << endl;
			dela(ps, [&](int i, int j, int k) {
				cout << i << ' ' << j << ' ' << k << endl;
			});

			abort();
		};

		ll sumar = 0;
		vector<int> used(n);
		bool any = false;
		dela(ps, [&](int i, int j, int k) {
			any = true;
			used[i] = used[j] = used[k] = 1;
			ll ar = ps[i].cross(ps[j], ps[k]);
			if (ar <= 0) fail();
			sumar += ar;
			floating_point_type c = cc_center(top(ps[i]), top(ps[j]), top(ps[k]));
			double ra = cc_radius(top(ps[i]), top(ps[j]), top(ps[k]));
			for (int l = 0; l < (n); ++l) {
				if ((top(ps[l]) - c).dist() < ra - 1e-5) fail();
			}
		});
		if (!all_colinear) {
			for (int i = 0; i < (n); ++i) if (!used[i]) fail();
		} else {
			assert(!any);
		}

		vector<integer_point> hull = convex_hull(ps);
		ll ar2 = polygon_area2(hull);
		if (ar2 != sumar) fail();

		continue; }
fail:;
	}
	cout<<"tests passed!"<<endl;
	// cerr << endl;
	return 0;
}

int main2() {
	vector<integer_point> ps;
	int n = 100000;
	int xrange = 20000;
	int yrange = 20000;
	for (int i = 0; i < (n); ++i) {
		ps.emplace_back(rand() % (2*xrange) - xrange, rand() % (2*yrange) - yrange);
	}
	sort(begin(ps), end(ps));
	ps.erase(unique(begin(ps), end(ps)), ps.end());

	cout << (int)(ps).size() << endl;
	triangulate(ps);
	return 0;
}

#ifdef TEST_PERF
int main() { return main2(); }
#else
int main() { return main1(); }
#endif
