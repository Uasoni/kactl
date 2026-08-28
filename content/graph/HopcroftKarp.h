/**
 * Author: adam soltan
 * Date: 2026-01-13
 * License: CC0
 * Description: fast bipartite matching algorithm. graph $g$ should be a list
 * of neighbors of the left partition, and $r$ should be a vector full of
 * $-1$'s of the same size as the right partition. returns the size of
 * the matching. $r[i]$ will be the match for vertex $i$ on the right side,
 * or $-1$ if it's not matched.
 * both partitions are numbered from 1; vectors include an unused index 0.
 * Time: O(E \sqrt{V})
 * Status: stress-tested by minimum_vertex_cover and tested on library checker
 */
#pragma once

int hopcroft_karp(vector<vector<int>>& g, vector<int>& r) {
	int n = (int)g.size() - 1, res = 0;
	vector<int> l(n + 1, -1), q(n + 1), d(n + 1);
	auto dfs = [&](auto f, int u) -> bool {
		int t = exchange(d[u], 0) + 1;
		for (int v : g[u])
			if (r[v] == -1 || (d[r[v]] == t && f(f, r[v])))
				return l[u] = v, r[v] = u, 1;
		return 0;
	};
	for (int t = 0, f = 0;; t = f = 0, d.assign(n + 1, 0)) {
		for (int i = 1; i <= n; ++i) if (l[i] == -1) q[t++] = i, d[i] = 1;
		for (int i = 0; i < (t); ++i) for (int v : g[q[i]]) {
			if (r[v] == -1) f = 1;
			else if (!d[r[v]]) d[r[v]] = d[q[i]] + 1, q[t++] = r[v];
		}
		if (!f) return res;
		for (int i = 1; i <= n; ++i) if (l[i] == -1) res += dfs(dfs, i);
	}
}
