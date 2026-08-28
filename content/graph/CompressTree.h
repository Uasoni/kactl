/**
 * Author: simon lindholm
 * Date: 2016-01-14
 * License: CC0
 * Description: given a rooted tree and a subset S of nodes, compute the minimal
 * subtree that contains all the nodes by adding all (at most $|S|-1$)
 * pairwise lca's and compressing edges.
 * returns a 1-indexed list of (parent, original\_index), with index 0 unused.
 * the root is at index 1 and points to itself.
 * Time: $O(|S| \log |S|)$
 * Status: tested at code_forces
 */
#pragma once

#include "LCA.h"
vector<pair<int, int>> compress_tree(LcaQuery& lca, const vector<int>& subset) {
	if (subset.empty()) return vector<pair<int, int>>(1);
	static vector<int> reverse_index;
	reverse_index.resize(lca.size() + 1);
	vector<int> vertices = subset;
	auto cmp = [&](int a, int b) { return lca.entry_time(a) < lca.entry_time(b); };
	sort(begin(vertices), end(vertices), cmp);
	int m = (int)vertices.size() - 1;
	for (int i = 0; i < m; ++i) {
		int a = vertices[i], b = vertices[i + 1];
		vertices.push_back(lca.query(a, b));
	}
	sort(begin(vertices), end(vertices), cmp);
	vertices.erase(unique(begin(vertices), end(vertices)), end(vertices));
	for (int i = 0; i < (int)vertices.size(); ++i)
		reverse_index[vertices[i]] = i + 1;
	vector<pair<int, int>> result(1);
	result.emplace_back(1, vertices[0]);
	for (int i = 0; i + 1 < (int)vertices.size(); ++i) {
		int a = vertices[i], b = vertices[i + 1];
		result.emplace_back(reverse_index[lca.query(a, b)], b);
	}
	return result;
}
