/**
 * Author: simon lindholm
 * Date: 2016-12-15
 * License: CC0
 * Source: http://en.wikipedia.org/wiki/floyd–warshall_algorithm
 * Description: calculates all-pairs shortest path in a directed graph that might have negative edge weights.
 * input is a 1-indexed distance matrix $m$, where $m[i][j] = \texttt{inf}$ if $i$ and $j$ are not adjacent.
 * as output, $m[i][j]$ is set to the shortest distance between $i$ and $j$, \texttt{inf} if no path,
 * or \texttt{-inf} if the path goes through a negative-weight cycle.
 * Time: O(N^3)
 * Status: slightly tested
 */
#pragma once

const ll INF = 1LL << 62;
void floyd_warshall(vector<vector<ll>>& m) {
	int n = (int)m.size() - 1;
	for (int i = 1; i <= n; ++i) m[i][i] = min(m[i][i], 0LL);
	for (int k = 1; k <= n; ++k) for (int i = 1; i <= n; ++i) for (int j = 1; j <= n; ++j)
		if (m[i][k] != INF && m[k][j] != INF) {
			auto new_dist = max(m[i][k] + m[k][j], -INF);
			m[i][j] = min(m[i][j], new_dist);
		}
	for (int k = 1; k <= n; ++k) if (m[k][k] < 0) for (int i = 1; i <= n; ++i) for (int j = 1; j <= n; ++j)
		if (m[i][k] != INF && m[k][j] != INF) m[i][j] = -INF;
}
