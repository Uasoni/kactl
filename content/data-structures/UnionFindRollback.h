/**
 * Author: lukas polacek, simon lindholm
 * Date: 2019-12-26
 * License: CC0
 * Source: folklore
 * Description: disjoint-set data structure with undo.
 * elements are numbered $1..n$.
 * if undo is not needed, skip st, time() and rollback().
 * Usage: int t = uf.time(); ...; uf.rollback(t);
 * Time: $O(\log(N))$
 * Status: tested as part of directed_mst.h
 */
#pragma once

struct RollbackUnionFind {
	vector<int> e; vector<pii> st;
	RollbackUnionFind(int n) : e(n + 1, -1) {}
	int size(int x) { return -e[find(x)]; }
	int find(int x) { return e[x] < 0 ? x : find(e[x]); }
	int time() { return (int)(st).size(); }
	void rollback(int t) {
		for (int i = time(); i --> t;)
			e[st[i].first] = st[i].second;
		st.resize(t);
	}
	bool join(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		st.push_back({a, e[a]});
		st.push_back({b, e[b]});
		e[a] += e[b]; e[b] = a;
		return true;
	}
};
