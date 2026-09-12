/**
 * Author: benjamin qi, oleksandr kulkov, chilli
 * Date: 2020-01-12
 * License: CC0
 * Source: https://codeforces.com/blog/entry/53170, https://github.com/bqi343/USACO/blob/master/implementations/content/graphs%20(12)/trees%20(10)/HeavyLightDecomposition%20(10.3).h
 * Description: decomposes a tree into vertex disjoint heavy paths and light
 * edges such that the path from any leaf to the root contains at most log(n)
 * light edges. code does additive modifications and max queries, but can
 * support commutative segtree modifications/queries on paths and subtrees.
 * takes as input the full adjacency list. VALUES\_ON\_EDGES being true means that
 * values are stored in the edges, as opposed to the nodes. all values
 * initialized to the segtree default. vertices are $1..n$, root is 1, and the
 * adjacency list has size $n+1$.
 * Initialize with tree->set(1,n,0) before additive updates (default is $-10^9$).
 * To load individual values, set position pos[v]; an edge's value belongs to
 * its deeper endpoint. Edge-mode path queries exclude the LCA, and subtree
 * queries exclude the edge to the parent. Empty queries return $-10^9$.
 * To change max to sum, adapt the segment tree (including lazy length factors)
 * and query identity/combine here. Noncommutative queries need path direction.
 * Usage: HeavyLightDecomposition<false> h(adj); // vertex values
 *  h.tree->set(1, n, 0);
 *  h.modify_path(u, v, 3); // add 3, including both endpoints
 *  int best = h.query_path(u, v);
 * Time: O((\log N)^2)
 * Status: stress-tested against old HeavyLightDecomposition
 */
#pragma once

#include "../data-structures/LazySegmentTree.h"

template <bool VALUES_ON_EDGES> struct HeavyLightDecomposition {
	int vertex_count, timer = 1;
	vector<vector<int>> adj;
	vector<int> par, siz, rt, pos;
	LazySegmentTree *tree;
	HeavyLightDecomposition(vector<vector<int>> adj_)
		: vertex_count((int)adj_.size() - 1), adj(adj_), par(adj_.size(), -1),
		  siz(adj_.size(), 1), rt(adj_.size()), pos(adj_.size()),
		  tree(new LazySegmentTree(1, vertex_count)) {
		rt[1] = 1; dfs_sz(1); dfs_hld(1);
	}
	void dfs_sz(int v) {
		for (int& u : adj[v]) {
			adj[u].erase(find(begin(adj[u]), end(adj[u]), v));
			par[u] = v;
			dfs_sz(u);
			siz[v] += siz[u];
			if (siz[u] > siz[adj[v][0]]) swap(u, adj[v][0]);
		}
	}
	void dfs_hld(int v) {
		pos[v] = timer++;
		for (int u : adj[v]) {
			rt[u] = (u == adj[v][0] ? rt[v] : u);
			dfs_hld(u);
		}
	}
	template <class B> void process(int u, int v, B op) {
		for (;; v = par[rt[v]]) {
			if (pos[u] > pos[v]) swap(u, v);
			if (rt[u] == rt[v]) break;
			op(pos[rt[v]], pos[v]);
		}
		// Skip LCA's incoming edge in edge mode.
		if (pos[u] + VALUES_ON_EDGES <= pos[v])
			op(pos[u] + VALUES_ON_EDGES, pos[v]);
	}
	void modify_path(int u, int v, int val) {
		process(u, v, [&](int l, int r) { tree->add(l, r, val); });
	}
	int query_path(int u, int v) { // modify depending on problem
		int res = -1e9;
		process(u, v, [&](int l, int r) {
				res = max(res, tree->query(l, r));
		});
		return res;
	}
	int query_subtree(int v) { // modify_subtree is similar
		return tree->query(pos[v] + VALUES_ON_EDGES, pos[v] + siz[v] - 1);
	}
};
