#include "../utilities/template.h"
#include "../utilities/utils.h"
#include "../utilities/random.h"

#include "../../content/graph/WeightedMatching.h"
#include <bits/extc++.h> /// include-line, keep-include
#include "../../content/graph/MinCostMaxFlow.h"

void test(int max_n, int mx_cost, int iters) {
	for (int it = 0; it < iters; it++) {
		int n = rand_range(0, max_n), m = rand_range(0, max_n);
		if (n > m)
			swap(n, m);

		MinCostMaxFlowGraph mcmf(n + m + 2);
		int s = 1;
		int t = 2;
		for (int i = 0; i < n; i++)
			mcmf.add_edge(s, i + 3, 1, 0);
		for (int i = 0; i < m; i++)
			mcmf.add_edge(3 + n + i, t, 1, 0);

		vector<vector<int>> cost(n, vector<int>(m));
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				cost[i][j] = rand_range(-mx_cost, mx_cost);
				mcmf.add_edge(i + 3, 3 + n + j, 1, cost[i][j]);
			}
		}
		mcmf.setpi(s);
		auto maxflow = mcmf.maxflow(s, t);
		auto matching = hungarian(cost);
		assert(maxflow.first == n);
		assert(maxflow.second == matching.first);
		int match_sum = 0;
		set<int> used;
		for (int i = 0; i < n; i++) {
			match_sum += cost[i][matching.second[i]];
			assert(used.count(matching.second[i]) == 0);
			used.insert(matching.second[i]);
		}
		assert(match_sum == matching.first);
		return;
	}
}
signed main() {
	test(25, 5, 1000);
	test(100, 1000, 100);
	test(100, 1, 50);
	test(5, 5, 10000);
	cout << "tests passed!" << endl;
}
