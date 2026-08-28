#include "../utilities/template.h"

#include "../../content/geometry/CircleIntersection.h"
typedef Point<double> point_type;

int main() {
	cin.sync_with_stdio(0); cin.tie(0);
	cin.exceptions(cin.failbit);
	srand(2);
	for (int it = 0; it < (100000); ++it) {
		double rnd[6];
		for (int i = 0; i < (6); ++i)
			rnd[i] = rand() % 21 - 10;
		point_type a(rnd[0], rnd[1]);
		point_type b(rnd[2], rnd[3]);
		double ra = rand() % 10;
		double rb = rand() % 10;
		if (a == b) continue;
		pair<point_type, point_type> out;
		bool ret = circle_inter(a, b, ra, rb, &out);
		if (ret) {
			assert(abs((out.first - a).dist() - ra) < 1e-9);
			assert(abs((out.second - a).dist() - ra) < 1e-9);
			assert(abs((out.first - b).dist() - rb) < 1e-9);
			assert(abs((out.second - b).dist() - rb) < 1e-9);
		}

		// hill-climb the answer
		auto func = [&](point_type x) {
			double d1 = (x - a).dist() - ra;
			double d2 = (x - b).dist() - rb;
			return d1*d1 + d2*d2;
		};
		point_type start = (a + b) / 2 + (a - b).perp();
		pair<double, point_type> cur(func(start), start);
		for (double jmp = 100; jmp > 1e-20; jmp /= 2) {
			int iters = 0;
			for (int imp = 1; imp--;) {
				if (++iters == 100) goto skip;
				for (int dx = -1; dx < (2); ++dx) for (int dy = -1; dy < (2); ++dy) {
					point_type p = cur.second;
					p.x += dx*jmp;
					p.y += dy*jmp;
					pair<double, point_type> np{func(p), p};
					if (np < cur) cur = np, imp = 1;
				}
			}
		}

		if (abs((cur.second - a).dist() - ra) < 1e-9 &&
		    abs((cur.second - b).dist() - rb) < 1e-9) {
			assert(ret);
			assert((out.first - cur.second).dist() < 1e-6 || (out.second - cur.second).dist() < 1e-6);
		} else {
			assert(!ret);
		}

		// cerr << '.';
		continue;
skip:;
		// sometimes hill-climbing is slow, for some reason. :(
		// cerr << '#';
	}
	cout<<"tests passed!"<<endl;
}
