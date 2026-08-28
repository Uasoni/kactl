/**
 * Author: johan sannemo, simon lindholm
 * Date: 2016-12-15
 * License: CC0
 * Description: finds a minimum vertex cover in a bipartite graph.
 *  the size is the same as the size of a maximum matching, and
 *  the complement is a maximum independent set.
 * left vertices are $1..n$, right vertices are $1..m$ in the graph, and
 * returned right vertices are shifted to $n+1..n+m$.
 * Status: stress-tested
 */
#pragma once

#include "DFSMatching.h"

vector<int> cover(vector<vector<int>>& g, int n, int m) {
	vector<int> match(m + 1, -1);
	int res = dfs_matching(g, match);
	vector<bool> lfound(n + 1, true), seen(m + 1);
	for (int i = 1; i <= m; ++i) if (match[i] != -1) lfound[match[i]] = false;
	vector<int> q, cover;
	for (int i = 1; i <= n; ++i) if (lfound[i]) q.push_back(i);
	while (!q.empty()) {
		int i = q.back(); q.pop_back();
		lfound[i] = 1;
		for (int e : g[i]) if (!seen[e] && match[e] != -1) {
			seen[e] = true;
			q.push_back(match[e]);
		}
	}
	for (int i = 1; i <= n; ++i) if (!lfound[i]) cover.push_back(i);
	for (int i = 1; i <= m; ++i) if (seen[i]) cover.push_back(n+i);
	assert((int)(cover).size() == res);
	return cover;
}
