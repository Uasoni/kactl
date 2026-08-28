#include "../utilities/template.h"

#include "../../content/geometry/MinimumEnclosingCircle.h"
typedef Point<double> point_type;

int main() {
	srand(2);
	for (int it = 0; it < (1000000); ++it) {
		int n = rand() % 20 + 1;
		// int n = 4;
		vector<point_type> ps;
		for (int i = 0; i < (n); ++i) {
			ps.emplace_back(rand() % 21 - 10, rand() % 21 - 10);
		}

		pair<point_type, double> pa = minimum_enclosing_circle(ps);
		point_type mid = pa.first;
		double rad = pa.second;
		double max_dist = 0;
		for(auto &p: ps) {
			max_dist = max(max_dist, (p - mid).dist());
		}

		assert(abs(max_dist - rad) < 1e-6);

		for (int it2 = 0; it2 < (50); ++it2) {
			point_type q2 = mid - point_type(0, 1e-6).rotate(it2);
			for(auto &p: ps) {
				if((p - q2).dist() > rad - 1e-7) goto fail;
			}
			assert(0);
fail:;
		}
	}
	cout<<"tests passed!"<<endl;
}
