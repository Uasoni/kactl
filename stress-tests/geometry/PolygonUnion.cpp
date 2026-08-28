#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;

#include "../../content/geometry/Point.h"
#include "../../content/geometry/sideOf.h"
#include "../../content/geometry/PolygonArea.h"
#include "../../content/geometry/PolygonUnion.h"
#include "../utilities/genPolygon.h"
#include "../utilities/random.h"
typedef Point<double> point_type;

namespace blackhorse {

using db = double;
const db EPS = 1e-8;

struct UnionPoint {
	db x, y;
	UnionPoint(db x = 0, db y = 0) : x(x), y(y) {}
};

inline int sgn(db x) { return (x > EPS) - (x < -EPS); }

UnionPoint operator-(UnionPoint p1, UnionPoint p2) { return UnionPoint(p1.x - p2.x, p1.y - p2.y); }

db vect(UnionPoint p1, UnionPoint p2) { return p1.x * p2.y - p1.y * p2.x; }

db scal(UnionPoint p1, UnionPoint p2) { return p1.x * p2.x + p1.y * p2.y; }

db polygon_union(vector<UnionPoint> poly[], int n) {
	auto ratio = [](UnionPoint a, UnionPoint b, UnionPoint origin) {
		return !sgn(a.x - b.x) ? (origin.y - a.y) / (b.y - a.y) : (origin.x - a.x) / (b.x - a.x);
	};
	db ret = 0;
	for (int i = 0; i < n; ++i) {
		for (size_t v = 0; v < poly[i].size(); ++v) {
			UnionPoint a = poly[i][v], b = poly[i][(v + 1) % poly[i].size()];
			vector<pair<db, int>> segs;
			segs.emplace_back(0, 0), segs.emplace_back(1, 0);
			for (int j = 0; j < n; ++j)
				if (i != j) {
					for (size_t u = 0; u < poly[j].size(); ++u) {
						UnionPoint c = poly[j][u], d = poly[j][(u + 1) % poly[j].size()];
						int sc = sgn(vect(b - a, c - a)), sd = sgn(vect(b - a, d - a));
						if (!sc && !sd) {
							if (sgn(scal(b - a, d - c)) > 0 && i > j) {
								segs.emplace_back(ratio(a, b, c), 1), segs.emplace_back(ratio(a, b, d), -1);
							}
						} else {
							db sa = vect(d - c, a - c), sb = vect(d - c, b - c);
							if (sc >= 0 && sd < 0)
								segs.emplace_back(sa / (sa - sb), 1);
							else if (sc < 0 && sd >= 0)
								segs.emplace_back(sa / (sa - sb), -1);
						}
					}
				}
			sort(segs.begin(), segs.end());
			db pre = min(max(segs[0].first, 0.0), 1.0), now, sum = 0;
			int cnt = segs[0].second;
			for (size_t j = 1; j < segs.size(); ++j) {
				now = min(max(segs[j].first, 0.0), 1.0);
				if (!cnt)
					sum += now - pre;
				cnt += segs[j].second;
				pre = now;
			}
			ret += vect(a, b) * sum;
		}
	}
	return ret / 2;
}
} // namespace blackhorse

namespace approximate {
#include "../../content/geometry/InsidePolygon.h"
double polygon_union(vector<vector<point_type>> &polygons, int lim) {
	int cnt = 0;
	int total = 0;
	for (double y = -lim + 1e-5; y < lim; y += lim / 500.0) {
		for (double x = -lim + 1.1e-5; x < lim; x += lim / 500.0) {
			total++;
			for (auto &i : polygons) {
				if (in_polygon(i, point_type(x, y))) {
					cnt++;
					break;
				}
			}
		}
	}
	return lim * lim * 4 * cnt / double(total);
}
} // namespace approximate

namespace lovelive {
typedef double db;
const db PI = acos(db(-1));
inline int sgn(db x) { return (x > 1e-8) - (x < -1e-8); }

typedef complex<db> cpoi;
db polygon_union(vector<cpoi> py[], int n) {
	auto ratio = [](cpoi &a, cpoi &b, cpoi &c) {
		cpoi x = b - a, y = c - a;
		if (sgn(real(x)) == 0)
			return imag(y) / imag(x);
		return real(y) / real(x);
	};
	db ret = 0;
	for (int i = 0; i < n; ++i)
		for (size_t v = 0; v < py[i].size(); ++v) {
			cpoi a = py[i][v], b = py[i][(v + 1) % py[i].size()];
			vector<pair<db, int>> segs = {{0, 0}, {1, 0}};
			for (int j = 0; j < n; ++j)
				if (i != j)
					for (size_t u = 0; u < py[j].size(); ++u) {
						cpoi c = py[j][u], d = py[j][(u + 1) % py[j].size()];
						int sc = sgn(imag(conj(b - a) * (c - a)));
						int sd = sgn(imag(conj(b - a) * (d - a)));
						if (!sc && !sd) {
							if (sgn(real(conj(b - a) * (d - c))) > 0 && i > j) {
								segs.push_back({ratio(a, b, c), +1});
								segs.push_back({ratio(a, b, d), -1});
							}
						} else {
							db sa = imag(conj(d - c) * (a - c));
							db sb = imag(conj(d - c) * (b - c));
							if (sc >= 0 && sd < 0)
								segs.push_back({sa / (sa - sb), 1});
							else if (sc < 0 && sd >= 0)
								segs.push_back({sa / (sa - sb), -1});
						}
					}
			sort(segs.begin(), segs.end());
			db pre = min(max(segs[0].first, 0.0), 1.0);
			db cur, sum = 0;
			int cnt = segs[0].second;
			for (size_t j = 1; j < segs.size(); ++j) {
				cur = min(max(segs[j].first, 0.0), 1.0);
				if (!cnt)
					sum += cur - pre;
				cnt += segs[j].second;
				pre = cur;
			}
			ret += imag(conj(a) * b) * sum;
		}
	ret = abs(ret) * 0.5;
	return ret;
}
} // namespace lovelive

point_type rand_pt(int lim) { return point_type(rand_range(-lim, lim), rand_range(-lim, lim)); }

point_type rnd_ulp(int lim, long long ulps = 5) { return point_type(rand_near_int_ulps(lim, ulps), rand_near_int_ulps(lim, ulps)); }

point_type rnd_eps(int lim, double eps) { return point_type(rand_near_int_eps(lim, eps), rand_near_int_eps(lim, eps)); }

void test_random(int n, int num_pts = 10, int lim = 5, bool brute = false) {
	vector<vector<point_type>> polygons;
	for (int i = 0; i < n; i++) {
		vector<point_type> pts;
		int k = rand_incl(3, num_pts);
		for (int j = 0; j < k; j++) {
			pts.push_back(rand_pt(lim)); // rnd_eps(lim, 1e-10));
		}
		polygons.push_back(gen_polygon(pts));
		if (polygon_area2(polygons.back()) < 0) {
			reverse(begin(polygons.back()), end(polygons.back()));
		}
	}
	auto val1 = poly_union(polygons);
	vector<vector<blackhorse::UnionPoint>> polygons2;
	for (auto i : polygons) {
		vector<blackhorse::UnionPoint> t;
		for (auto j : i)
			t.push_back({j.x, j.y});
		polygons2.push_back(t);
	}
	vector<vector<lovelive::cpoi>> polygons3;
	for (auto i : polygons) {
		vector<lovelive::cpoi> t;
		for (auto j : i)
			t.push_back({j.x, j.y});
		polygons3.push_back(t);
	}
	auto val3 = blackhorse::polygon_union(polygons2.data(), (int)(polygons2).size());
	auto val4 = lovelive::polygon_union(polygons3.data(), (int)(polygons3).size());
	if (abs(val1 - val3) > 1e-8 || abs(val1 - val4) > 1e-8) {
		for (int i = 0; i < (n); ++i) {
			for (auto &x : polygons[i]) {
				cout << x << ' ';
			}
			cout << endl;
		}
		abort();
	}
}

int main() {
	// int s = (int)time(0);
	int s = 1;
	// cout << "seed " << s << endl;
	srand(s);
	for (int i = 0; i < 100; i++) {
		test_random(2, 5, 5);
	}
	for (int i = 0; i < 100; i++) {
		test_random(2, 10, 2);
	}
	for (int i = 0; i < 50; i++) {
		test_random(5, 100, 5);
	}
	cout << "tests passed!" << endl;
}
