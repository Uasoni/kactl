/**
 * Author: benjamin qi, chilli
 * Date: 2020-04-04
 * License: CC0
 * Source: https://github.com/bqi343/USACO/blob/master/implementations/content/graphs%20(12)/matching/hungarian.h
 * Description: given a weighted bipartite graph, matches every node on
 * the left with a node on the right such that no
 * nodes are in two matchings and the sum of the edge weights is minimal. takes
 * a 1-indexed cost matrix, where cost[i][j] is the cost for L[i] to be matched with R[j] and
 * returns (min cost, match), where L[i] is matched with
 * R[match[i]]. negate costs for max cost. requires $N \le M$.
 * Time: O(N^2M)
 * Status: tested on kattis:cordonbleu, stress-tested
 */
#pragma once

pair<int, vector<int>> hungarian(const vector<vector<int>>& cost) {
	if (cost.size() <= 1) return {0, vector<int>(1)};
	int n = (int)cost.size() - 1, m = (int)cost[1].size() - 1;
	vector<int> u(n + 1), v(m + 1), p(m + 1), answer(n + 1);
	for (int i = 1; i <= n; ++i) {
		p[0] = i;
		int j0 = 0; // add "dummy" worker 0
		vector<int> dist(m + 1, INT_MAX), pre(m + 1, -1);
		vector<bool> done(m + 1);
		do { // dijkstra
			done[j0] = true;
			int i0 = p[j0], j1, delta = INT_MAX;
			for (int j = 1; j <= m; ++j) if (!done[j]) {
				auto cur = cost[i0][j] - u[i0] - v[j];
				if (cur < dist[j]) dist[j] = cur, pre[j] = j0;
				if (dist[j] < delta) delta = dist[j], j1 = j;
			}
			for (int j = 0; j <= m; ++j) {
				if (done[j]) u[p[j]] += delta, v[j] -= delta;
				else dist[j] -= delta;
			}
			j0 = j1;
		} while (p[j0]);
		while (j0) { // update alternating path
			int j1 = pre[j0];
			p[j0] = p[j1], j0 = j1;
		}
	}
	for (int j = 1; j <= m; ++j) if (p[j]) answer[p[j]] = j;
	return {-v[0], answer}; // min cost
}
