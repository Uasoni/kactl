/**
 * Author: simon lindholm
 * Date: 2020-10-12
 * License: CC0
 * Source: https://en.wikipedia.org/wiki/misra_%26_Gries_edge_coloring_algorithm
 * https://codeforces.com/blog/entry/75431 for the note about bipartite graphs.
 * Description: given a simple, undirected graph with max degree $D$, computes a
 * $(D + 1)$-coloring of the edges such that no neighboring edges share a color.
 * ($D$-coloring is NP-hard, but can be done for bipartite graphs by repeated matchings of
 * max-degree nodes.)
 * vertices and returned edge colors are numbered from 1. The result has an
 * unused index 0, so result[i] is the color of edges[i-1].
 * Time: O(NM)
 * Status: stress-tested, tested on kattis:gamescheduling
 */
#pragma once

vector<int> edge_coloring(int n, vector<pii> eds) {
	vector<int> cc(n + 2), ret(eds.size() + 1), fan(n + 1), free(n + 1), loc;
	for (pii e : eds) ++cc[e.first], ++cc[e.second];
	int u, v, ncols = *max_element(begin(cc), end(cc)) + 1;
	vector<vector<int>> adj(n + 1, vector<int>(ncols, -1));
	for (pii e : eds) {
		tie(u, v) = e;
		fan[0] = v;
		loc.assign(ncols, 0);
		int at = u, end = u, d, c = free[u], ind = 0, i = 0;
		while (d = free[v], !loc[d] && (v = adj[u][d]) != -1)
			loc[d] = ++ind, cc[ind] = d, fan[ind] = v;
		cc[loc[d]] = c;
		for (int cd = d; at != -1; cd ^= c ^ d, at = adj[at][cd])
			swap(adj[at][cd], adj[end = at][cd ^ c ^ d]);
		while (adj[fan[i]][d] != -1) {
			int left = fan[i], right = fan[++i], e = cc[i];
			adj[u][e] = left;
			adj[left][e] = u;
			adj[right][e] = -1;
			free[right] = e;
		}
		adj[u][d] = fan[i];
		adj[fan[i]][d] = u;
		for (int y : {fan[0], u, end})
			for (int& z = free[y] = 0; adj[y][z] != -1; z++);
	}
	for (int i = 1; i <= (int)eds.size(); ++i) {
		int& color = ret[i];
		for (tie(u, v) = eds[i-1]; adj[u][color] != v;) ++color;
		++color;
	}
	return ret;
}
