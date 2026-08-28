/**
 * Author: simon lindholm
 * Date: 2019-12-31
 * License: CC0
 * Source: folklore
 * Description: eulerian undirected/directed path/cycle algorithm.
 * input should be a vector of (dest, global edge index), where
 * for undirected graphs, forward/backward edges have the same index.
 * returns a list of nodes in the eulerian path/cycle with src at both start and end, or
 * empty list if no cycle/path exists.
 * vertices and global edge indices are numbered from 1; the adjacency list
 * has size $n+1$.
 * to get edge indices back, add .second to s and ret.
 * Time: O(V + E)
 * Status: stress-tested
 */
#pragma once

vector<int> euler_walk(vector<vector<pii>>& gr, int edge_count, int src=1) {
	int n = (int)(gr).size();
	vector<int> degree_delta(n), its(n), used(edge_count + 1), ret, s = {src};
	degree_delta[src]++; // to allow euler paths, not just cycles
	while (!s.empty()) {
		int x = s.back(), y, e, &it = its[x], end = (int)(gr[x]).size();
		if (it == end){ ret.push_back(x); s.pop_back(); continue; }
		tie(y, e) = gr[x][it++];
		if (!used[e]) {
			degree_delta[x]--, degree_delta[y]++;
			used[e] = 1; s.push_back(y);
		}}
	for (int i = 1; i < n; ++i)
		if (degree_delta[i] < 0 || (int)ret.size() != edge_count+1) return {};
	return {ret.rbegin(), ret.rend()};
}
