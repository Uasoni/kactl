#include "../utilities/template.h"

#include "../../content/geometry/LineProjectionReflection.h"
#include "../../content/geometry/lineDistance.h"

typedef Point<double> point_type;
int main() {
	cin.sync_with_stdio(0);
	cin.tie(0);
	const int lim = 5;
	for (int i = 0; i < 100000; i++) {
		point_type p = point_type(rand() % lim, rand() % lim);
		point_type a = point_type(rand() % lim, rand() % lim);
		point_type b = point_type(rand() % lim, rand() % lim);
		while (a == b)
			b = point_type(rand() % lim, rand() % lim);
		auto proj = line_proj(a, b, p, false);
		auto refl = line_proj(a, b, p, true);
		assert(line_dist(a, b, proj) < 1e-8);
		auto man_proj = (refl + p) / 2;
		assert((proj-man_proj).dist() < 1e-8);
	}
	cout<<"tests passed!"<<endl;
}
