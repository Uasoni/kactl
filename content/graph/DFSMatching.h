/**
 * Author: lukas polacek
 * Date: 2009-10-28
 * License: CC0
 * Source:
 * Description: simple bipartite matching algorithm. graph $g$ should be a list
 * of neighbors of the left partition, and $btoa$ should be a vector full of
 * -1's of the same size as the right partition. returns the size of
 * the matching. $btoa[i]$ will be the match for vertex $i$ on the right side,
 * or $-1$ if it's not matched.
 * both partitions are numbered from 1; vectors include an unused index 0.
 * Time: O(VE)
 * Usage: vector<int> btoa(m + 1, -1); dfs_matching(g, btoa);
 * Status: works
 */
#pragma once

bool find_augmenting_path(int j, vector<vector<int>>& g, vector<int>& btoa,
		vector<int>& vis) {
	if (btoa[j] == -1) return 1;
	vis[j] = 1; int di = btoa[j];
	for (int e : g[di])
		if (!vis[e] && find_augmenting_path(e, g, btoa, vis)) {
			btoa[e] = di;
			return 1;
		}
	return 0;
}
int dfs_matching(vector<vector<int>>& g, vector<int>& btoa) {
	vector<int> vis;
	for (int i = 1; i < (int)g.size(); ++i) {
		vis.assign((int)(btoa).size(), 0);
		for (int j : g[i])
			if (find_augmenting_path(j, g, btoa, vis)) {
				btoa[j] = i;
				break;
			}
	}
	return (int)btoa.size() - 1
		- (int)count(next(begin(btoa)), end(btoa), -1);
}
