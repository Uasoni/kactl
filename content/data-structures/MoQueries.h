/**
 * Author: simon lindholm
 * Date: 2019-12-28
 * License: CC0
 * Source: https://github.com/hoke-t/tamu-kactl/blob/master/content/data-structures/mo_queries.h
 * Description: answer interval or tree path queries by finding an approximate TSP through the queries,
 * and moving from one query to the next by adding/removing points at the ends.
 * if values are on tree edges, change \texttt{step} to add/remove the edge $(a, c)$ and remove the initial \texttt{add} call (but keep \texttt{in}).
 * Time: O(N \sqrt Q)
 * Status: stress-tested
 */
#pragma once

void add(int ind, int end) { ... } // add a[ind] (end = 0 or 1)
void del(int ind, int end) { ... } // remove a[ind]
int calc() { ... } // compute current answer

vector<int> mo(vector<pii> queries) {
	int left = 1, right = 0, block_size = 350; // ~N/sqrt(Q)
	vector<int> order((int)queries.size()), result = order;
	auto key = [&](pii query) {
		int block = query.first / block_size;
		return pii(block, query.second ^ -(block & 1));
	};
	iota(begin(order), end(order), 0);
	sort(begin(order), end(order), [&](int a, int b) {
		return key(queries[a]) < key(queries[b]);
	});
	for (int qi : order) {
		pii q = queries[qi];
		while (left > q.first) add(--left, 0);
		while (right < q.second) add(++right, 1);
		while (left < q.first) del(left++, 0);
		while (right > q.second) del(right--, 1);
		result[qi] = calc();
	}
	return result;
}

vector<int> mo_tree(vector<array<int, 2>> queries, vector<vector<int>>& ed, int root=1){
	int n = (int)ed.size() - 1, timer = 0, pos[2] = {root, root}, block_size = 350;
	vector<int> order((int)queries.size()), result = order, index(n + 1), path(n + 1);
	vector<int> left(n + 1), right(n + 1), in(n + 1), par(n + 1);
	add(root, 0), in[root] = 1;
	auto dfs = [&](int x, int p, int dep, auto& f) -> void {
		par[x] = p;
		left[x] = timer;
		if (dep) index[x] = ++timer;
		for (int y : ed[x]) if (y != p) f(y, x, !dep, f);
		if (!dep) index[x] = ++timer;
		right[x] = timer;
	};
	dfs(root, -1, 0, dfs);
	auto key = [&](array<int, 2> query) {
		int block = index[query[0]] / block_size;
		return pii(block, index[query[1]] ^ -(block & 1));
	};
	iota(begin(order), end(order), 0);
	sort(begin(order), end(order), [&](int a, int b) {
		return key(queries[a]) < key(queries[b]);
	});
	for (int qi : order) for (int end = 0; end < 2; ++end) {
		int &a = pos[end], b = queries[qi][end], i = 0;
		auto step = [&](int c) {
			if (in[c]) { del(a, end); in[a] = 0; }
			else { add(c, end); in[c] = 1; }
			a = c;
		};
		while (!(left[b] <= left[a] && right[a] <= right[b]))
			path[++i] = b, b = par[b];
		while (a != b) step(par[a]);
		while (i) step(path[i--]);
		if (end) result[qi] = calc();
	}
	return result;
}
