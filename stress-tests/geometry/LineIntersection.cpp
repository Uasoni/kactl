#include "../utilities/template.h"

#include "../../content/geometry/lineIntersection.h"
#include "../../content/geometry/lineDistance.h"

int main() {
	for (int t = 0; t < (1000000); ++t) {
		const int GRID=10;
		Point<double>
			a(rand()%GRID, rand()%GRID),
			b(rand()%GRID, rand()%GRID),
			c(rand()%GRID, rand()%GRID),
			d(rand()%GRID, rand()%GRID);
		auto pa = line_inter(a,b,c,d);
		if (pa.first == 1) {
			assert(line_dist(a, b, pa.second) < 1e-8);
			assert(line_dist(c, d, pa.second) < 1e-8);
		}
	}
	cout<<"tests passed!"<<endl;
}
