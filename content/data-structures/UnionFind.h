/**
 * Author: lukas polacek
 * Date: 2009-10-26
 * License: CC0
 * Source: folklore
 * Description: disjoint-set data structure.
 * elements are numbered $1..n$.
 * Time: $O(\alpha(N))$
 */
#pragma once

struct UnionFind {
	vector<int> e;
	UnionFind(int n) : e(n + 1, -1) {}
	bool same_set(int a, int b) { return find(a) == find(b); }
	int size(int x) { return -e[find(x)]; }
	int find(int x) { return e[x] < 0 ? x : e[x] = find(e[x]); }
	bool join(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		e[a] += e[b]; e[b] = a;
		return true;
	}
};
