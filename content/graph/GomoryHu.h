/**
 * Author: chilli, takanori MAEHARA
 * Date: 2020-04-03
 * License: CC0
 * Source: https://github.com/spaghetti-source/algorithm/blob/master/graph/gomory_hu_tree.cc#L102
 * Description: given a list of edges representing an undirected flow graph,
 * returns edges of the gomory-hu tree. the max flow between any pair of
 * vertices is given by minimum array<ll, 3> weight along the gomory-hu tree path.
 * Time: $O(V)$ flow computations
 * Status: tested on CERC 2015 J, stress-tested
 *
 * Details: the implementation used here is not actually the original
 * gomory-hu, but gusfield's simplified version: "very simple methods for all
 * pairs network flow analysis". PushRelabel is used here, but any flow
 * implementation that supports `left_of_min_cut` also works.
 * vertices are numbered $1..n$.
 */
#pragma once

#include "PushRelabel.h"
vector<array<ll, 3>> gomory_hu(int n, vector<array<ll, 3>> edges) {
	vector<array<ll, 3>> tree;
	vector<int> parent(n + 1, 1);
	for (int i = 2; i <= n; ++i) {
		PushRelabel flow(n); // Dinic also works
		for (array<ll, 3> edge : edges)
			flow.add_edge((int)edge[0], (int)edge[1], edge[2], edge[2]);
		tree.push_back({i, parent[i], flow.calc(i, parent[i])});
		for (int j = i + 1; j <= n; ++j)
			if (parent[j] == parent[i] && flow.left_of_min_cut(j)) parent[j] = i;
	}
	return tree;
}
