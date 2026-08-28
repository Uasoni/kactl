/**
 * Author: simon lindholm
 * Date: 2021-01-09
 * License: CC0
 * Source: https://en.wikipedia.org/wiki/stoer%E2%80%93Wagner_algorithm
 * Description: find a global minimum cut in an undirected graph represented
 *  by a 1-indexed adjacency matrix. Returned vertices are numbered from 1.
 * Time: O(V^3)
 * Status: stress-tested together with gomory_hu
 */
#pragma once

pair<int, vector<int>> global_min_cut(const vector<vector<int>>& graph) {
	pair<int, vector<int>> best = {INT_MAX, {}};
	int n = (int)graph.size() - 1;
	vector<vector<int>> mat(n, vector<int>(n));
	for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
		mat[i][j] = graph[i+1][j+1];
	vector<vector<int>> co(n);
	for (int i = 0; i < n; ++i) co[i] = {i + 1};
	for (int ph = 1; ph < (n); ++ph) {
		vector<int> w = mat[0];
		size_t s = 0, t = 0;
		for (int it = 0; it < (n-ph); ++it) { // O(V^2) -> O(E log V) with prio. queue
			w[t] = INT_MIN;
			s = t, t = max_element(begin(w), end(w)) - w.begin();
			for (int i = 0; i < (n); ++i) w[i] += mat[t][i];
		}
		best = min(best, {w[t] - mat[t][t], co[t]});
		co[s].insert(co[s].end(), begin(co[t]), end(co[t]));
		for (int i = 0; i < (n); ++i) mat[s][i] += mat[t][i];
		for (int i = 0; i < (n); ++i) mat[i][s] = mat[s][i];
		mat[0][t] = INT_MIN;
	}
	return best;
}
