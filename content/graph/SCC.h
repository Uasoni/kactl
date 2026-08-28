/**
 * Author: lukas polacek
 * Date: 2009-10-28
 * License: CC0
 * Source: czech graph algorithms book, by demel. (tarjan's algorithm)
 * Description: finds strongly connected components in a
 * directed graph. if vertices $u, v$ belong to the same component,
 * we can reach $u$ from $v$ and vice versa.
 * Usage: scc(graph, [\&](vector<int>\& v) { ... }) visits all components
 * in reverse topological order. comp[i] holds the component
 * index of a node (a component only has edges to components with
 * lower index). Vertices and component indices are numbered from 1; the graph
 * and output vectors have an unused index 0.
 * Time: O(E + V)
 * Status: BruteForce-tested for N <= 5
 */
#pragma once

vector<int> val, comp, z, cont;
int dfs_time, component_count;
template<class G, class F> int scc_dfs(int j, G& g, F& f) {
	int low = val[j] = ++dfs_time, x; z.push_back(j);
	for (auto e : g[j]) if (comp[e] < 0)
		low = min(low, val[e] ? val[e] : scc_dfs(e, g, f));

	if (low == val[j]) {
		++component_count;
		do {
			x = z.back(); z.pop_back();
			comp[x] = component_count;
			cont.push_back(x);
		} while (x != j);
		f(cont); cont.clear();
	}
	return val[j] = low;
}
template<class G, class F> void scc(G& g, F f) {
	int n = (int)g.size() - 1;
	val.assign(n + 1, 0); comp.assign(n + 1, -1);
	dfs_time = component_count = 0;
	for (int i = 1; i <= n; ++i) if (comp[i] < 0) scc_dfs(i, g, f);
}
