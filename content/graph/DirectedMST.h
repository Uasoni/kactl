/**
 * Author: chilli, takanori MAEHARA, benq, simon lindholm
 * Date: 2019-05-10
 * License: CC0
 * Source: https://github.com/spaghetti-source/algorithm/blob/master/graph/arborescence.cc
 * and https://github.com/bqi343/USACO/blob/42d177dfb9d6ce350389583cfa71484eb8ae614c/implementations/content/graphs%20(12)/advanced/directed_mst.h for the reconstruction
 * Description: finds a minimum spanning
 * tree/arborescence of a directed graph. vertices are numbered $1..n$; the
 * returned parent vector also uses indices $1..n$. if no MST exists, returns -1.
 * Time: O(E \log V)
 * Status: stress-tested, also tested on NWERC 2018 fastestspeedrun
 */
#pragma once

#include "../data-structures/UnionFindRollback.h"

struct Edge { int a, b; ll w; };
struct HeapNode { /// lazy skew heap HeapNode
	Edge key;
	HeapNode *l, *r;
	ll delta;
	void prop() {
		key.w += delta;
		if (l) l->delta += delta;
		if (r) r->delta += delta;
		delta = 0;
	}
	Edge top() { prop(); return key; }
};
HeapNode *merge(HeapNode *a, HeapNode *b) {
	if (!a || !b) return a ?: b;
	a->prop(), b->prop();
	if (a->key.w > b->key.w) swap(a, b);
	swap(a->l, (a->r = merge(b, a->r)));
	return a;
}
void pop(HeapNode*& a) { a->prop(); a = merge(a->l, a->r); }

pair<ll, vector<int>> dmst(int n, int r, vector<Edge>& g) {
	RollbackUnionFind uf(n);
	vector<HeapNode*> heap(n + 1);
	for (Edge e : g) heap[e.b] = merge(heap[e.b], new HeapNode{e});
	ll res = 0;
	vector<int> seen(n + 1, -1), path(n + 1), par(n + 1);
	seen[r] = r;
	vector<Edge> chosen(n + 1), in(n + 1, {-1,-1}), comp;
	deque<tuple<int, int, vector<Edge>>> cycs;
	for (int s = 1; s <= n; ++s) {
		int u = s, qi = 0, w;
		while (seen[u] < 0) {
			if (!heap[u]) return {-1,{}};
			Edge e = heap[u]->top();
			heap[u]->delta -= e.w, pop(heap[u]);
			chosen[qi] = e, path[qi++] = u, seen[u] = s;
			res += e.w, u = uf.find(e.a);
			if (seen[u] == s) { /// found cycle, contract
				HeapNode* cyc = 0;
				int end = qi, time = uf.time();
				do cyc = merge(cyc, heap[w = path[--qi]]);
				while (uf.join(u, w));
				u = uf.find(u), heap[u] = cyc, seen[u] = -1;
				cycs.push_front({u, time, {&chosen[qi], &chosen[end]}});
			}
		}
		for (int i = 0; i < qi; ++i) in[uf.find(chosen[i].b)] = chosen[i];
	}

	for (auto& [u,t,comp] : cycs) { // restore sol (optional)
		uf.rollback(t);
		Edge in_edge = in[u];
		for (auto& e : comp) in[uf.find(e.b)] = e;
		in[uf.find(in_edge.b)] = in_edge;
	}
	for (int i = 1; i <= n; ++i) par[i] = in[i].a;
	return {res, par};
}
