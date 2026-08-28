#include "../utilities/template.h"
#include "../utilities/utils.h"

#include "../../content/graph/GlobalMinCut.h"
#include "../../content/graph/GomoryHu.h"
#include "../../content/graph/Dinic.h"


void test(int max_n, int max_flow, int iterations) {
	for (int it = 0; it < iterations; it++) {
		int n = rand() % max_n + 1;
		int m = rand() % (max_n * max_n);
		vector<array<ll, 3>> edges;
		vector<vector<int>> mat(n, vector<int>(n));
		for (int it = 0; it < (m); ++it) {
			int i = rand() % n;
			int j = rand() % n;
			if (i == j) continue;
			int w = rand() % max_flow;
			edges.push_back({i + 1, j + 1, w});
			mat[i][j] += w;
			mat[j][i] += w;
		}
		auto calc = [&](int s, int t) {
			Dinic flow(n);
			for (auto e : edges) {
				flow.add_edge((int)e[0], (int)e[1], e[2], e[2]);
			}
			return flow.calc(s, t);
		};
		vector<array<ll, 3>> gomory_hu_tree = gomory_hu(n, edges);
		vector<vector<array<int, 2>>> adj(n + 1);
		for (auto e : gomory_hu_tree) {
			adj[e[0]].push_back({(int)e[1], (int)e[2]});
			adj[e[1]].push_back({(int)e[0], (int)e[2]});
		}
		auto dfs = make_y_combinator([&](auto dfs, int start, int cur, int p, int mn) -> void {
			if (start != cur) {
				assert(mn == calc(start, cur));
			}
			for (auto i : adj[cur]) {
				if (i[0] != p)
					dfs(start, i[0], cur, min(mn, i[1]));
			}
		});
		dfs(1, 1, 0, INT_MAX);

		// check that the lightest edge agrees with global_min_cut.
		if (n >= 2) {
			ll min_cut = LLONG_MAX;
			for (auto e : gomory_hu_tree) {
				min_cut = min(min_cut, e[2]);
			}
			auto mat2 = mat;
			auto pa = global_min_cut(mat2);
			assert(pa.first == min_cut);
			vector<int> in_cut(n);
			assert((int)(pa.second).size() != 0);
			assert((int)(pa.second).size() != n);
			for (int x : pa.second) {
				assert(0 <= x && x < n);
				assert(!in_cut[x]);
				in_cut[x] = 1;
			}
			int cutw = 0;
			for (int i = 0; i < (n); ++i) for (int j = 0; j < (n); ++j) if (in_cut[i] && !in_cut[j]) {
				cutw += mat[i][j];
			}
			assert(pa.first == cutw);
		}
	}
}
signed main() {
	test(25, 5, 200);
	test(100, 1000, 5);
	test(100, 1, 20);
	test(5, 5, 20000);
	cout<<"tests passed!"<<endl;
}
