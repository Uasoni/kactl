/**
 * Author: chilli
 * Date: 2019-04-26
 * License: CC0
 * Source: https://cp-algorithms.com/graph/Dinic.html
 * Description: flow algorithm with complexity $O(VE\log U)$ where $U = \max |\text{cap}|$.
 * $O(\min(E^{1/2}, V^{2/3})E)$ if $U = 1$; $O(\sqrt{V}E)$ for bipartite matching.
 * vertices are numbered $1..n$.
 * Status: tested on SPOJ FASTFLOW and SPOJ MATCHING, stress-tested
 */
#pragma once

struct Dinic {
	struct Edge {
		int to, rev;
		ll c, oc;
		ll flow() { return max(oc - c, 0LL); } // if you need flows
	};
	vector<int> lvl, ptr, q;
	vector<vector<Edge>> adj;
	Dinic(int n) : lvl(n + 1), ptr(n + 1), q(n + 1), adj(n + 1) {}
	void add_edge(int a, int b, ll c, ll rcap = 0) {
		adj[a].push_back({b, (int)(adj[b]).size(), c, c});
		adj[b].push_back({a, (int)(adj[a]).size() - 1, rcap, rcap});
	}
	ll dfs(int v, int t, ll f) {
		if (v == t || !f) return f;
		for (int& i = ptr[v]; i < (int)(adj[v]).size(); i++) {
			Edge& e = adj[v][i];
			if (lvl[e.to] == lvl[v] + 1)
				if (ll p = dfs(e.to, t, min(f, e.c))) {
					e.c -= p, adj[e.to][e.rev].c += p;
					return p;
				}
		}
		return 0;
	}
	ll calc(int s, int t) {
		ll flow = 0; q[0] = s;
		for (int bit = 0; bit < 31; ++bit) do { // starting at bit=30 may be faster
			lvl = ptr = vector<int>((int)(q).size());
			int qi = 0, qe = lvl[s] = 1;
			while (qi < qe && !lvl[t]) {
				int v = q[qi++];
				for (Edge e : adj[v])
					if (!lvl[e.to] && e.c >> (30 - bit))
						q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
			}
			while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
		} while (lvl[t]);
		return flow;
	}

	bool left_of_min_cut(int a) { return lvl[a] != 0; }
};
