/**
 * Author: johan sannemo
 * Date: 2015-02-06
 * License: CC0
 * Source: folklore
 * Description: calculate power of two jumps in a tree,
 * to support fast upward jumps and LCAs.
 * vertices are numbered $1..n$, the parent vector has size $n+1$, and the
 * root points to itself.
 * Time: construction $O(N \log N)$, queries $O(\log N)$
 * Status: tested at petrozavodsk, also stress-tested via lca.cpp
 */
#pragma once

vector<vector<int>> tree_jump(const vector<int>& parent) {
	int on = 1, d = 1;
	while (on < (int)parent.size() - 1) on *= 2, d++;
	vector<vector<int>> jump(d, parent);
	for (int i = 1; i < d; ++i) for (int j = 1; j < (int)parent.size(); ++j)
		jump[i][j] = jump[i-1][jump[i-1][j]];
	return jump;
}

int jump(const vector<vector<int>>& table, int node, int steps) {
	for (int i = 0; i < (int)table.size(); ++i)
		if (steps & (1 << i)) node = table[i][node];
	return node;
}

int lca(const vector<vector<int>>& table, const vector<int>& depth, int a, int b) {
	if (depth[a] < depth[b]) swap(a, b);
	a = jump(table, a, depth[a] - depth[b]);
	if (a == b) return a;
	for (int i = (int)table.size(); i--;) {
		int c = table[i][a], d = table[i][b];
		if (c != d) a = c, b = d;
	}
	return table[0][a];
}
