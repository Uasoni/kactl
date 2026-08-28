#include "../utilities/template.h"

#include "../../content/geometry/Point.h"
#include "../../content/geometry/ManhattanMST.h"
#include "../../content/data-structures/UnionFind.h"


typedef Point<int> point_type;
typedef int T;
T rectilinear_mst_n(vector<point_type> ps) {
	struct Edge { int src, dst; T weight; };
	vector<Edge> edges;

	auto dist = [&](int i, int j) {
		return abs((ps[i]-ps[j]).x) + abs((ps[i]-ps[j]).y);
	};
	for (int i = 0; i < (int)(ps).size(); ++i)
		for (int j = i+1; j < (int)(ps).size(); ++j)
			edges.push_back({i, j, dist(i,j)});
	T cost = 0;
	sort(begin(edges), end(edges), [](Edge a, Edge b) { return a.weight < b.weight; });
	UnionFind uf((int)(ps).size());
	for (auto e: edges)
		if (uf.join(e.src, e.dst))
			cost += e.weight;
	return cost;
}

signed main() {
		for (int t=0; t<10000; t++) {
				const int max_coord = rand() % 300 + 1;
				const int num_pts = rand() % 100;
				vector<point_type> pts;
				for (int i = 0; i < num_pts; ++i) {
						int x = rand() % max_coord - max_coord / 2;
						int y = rand() % max_coord - max_coord / 2;
						pts.push_back(point_type(x,y));
				}
				auto edges = manhattan_mst(pts);
				assert(edges.size() <= 4*pts.size());
				sort(begin(edges), end(edges));
				UnionFind uf((int)(pts).size());
				int cost = 0, joined = 0;
				for (auto e: edges) if (uf.join(e[1], e[2])) cost += e[0], joined++;
				if (num_pts > 0) assert(joined == num_pts - 1);
				assert(cost == rectilinear_mst_n(pts));
		}
		cout<<"tests passed!"<<endl;
}
