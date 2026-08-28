/**
 * Author: simon lindholm
 * Date: 2015-02-24
 * License: CC0
 * Source: wikipedia, tiny_kactl
 * Description: push-relabel using the highest label selection rule and the gap heuristic. quite fast in practice.
 *  to obtain the actual flow, look at positive values only.
 * vertices are numbered $1..n$.
 * Time: $O(V^2\sqrt E)$
 * Status: tested on kattis and SPOJ, and stress-tested
 */
#pragma once


struct PushRelabel {
	struct Edge {
		int dest, back;
		ll f, c;
	};
	vector<vector<Edge>> g;
	vector<ll> ec;
	vector<Edge*> cur;
	vector<vector<int>> hs; vector<int> height;
	PushRelabel(int n) : g(n + 1), ec(n + 1), cur(n + 1),
		hs(2*n + 2), height(n + 1) {}

	void add_edge(int s, int t, ll cap, ll rcap=0) {
		if (s == t) return;
		g[s].push_back({t, (int)(g[t]).size(), 0, cap});
		g[t].push_back({s, (int)(g[s]).size()-1, 0, rcap});
	}

	void add_flow(Edge& e, ll f) {
		Edge &back = g[e.dest][e.back];
		if (!ec[e.dest] && f) hs[height[e.dest]].push_back(e.dest);
		e.f += f; e.c -= f; ec[e.dest] += f;
		back.f -= f; back.c += f; ec[back.dest] -= f;
	}
	ll calc(int s, int t) {
		const int INF_HEIGHT = 1000000000;
		int n = (int)g.size() - 1; height[s] = n; ec[t] = 1;
		vector<int> co(2*n + 2); co[0] = n-1;
		for (int i = 1; i <= n; ++i) cur[i] = g[i].data();
		for (Edge& e : g[s]) add_flow(e, e.c);

		for (int hi = 0;;) {
			while (hs[hi].empty()) if (!hi--) return -ec[s];
			int u = hs[hi].back(); hs[hi].pop_back();
			while (ec[u] > 0)  // discharge u
				if (cur[u] == g[u].data() + (int)(g[u]).size()) {
					height[u] = INF_HEIGHT;
					for (Edge& e : g[u]) if (e.c && height[u] > height[e.dest]+1)
						height[u] = height[e.dest]+1, cur[u] = &e;
					if (++co[height[u]], !--co[hi] && hi < n)
						for (int i = 1; i <= n; ++i) if (hi < height[i] && height[i] < n)
							--co[height[i]], height[i] = n + 1;
					hi = height[u];
				} else if (cur[u]->c && height[u] == height[cur[u]->dest]+1)
					add_flow(*cur[u], min(ec[u], cur[u]->c));
				else ++cur[u];
		}
	}
	bool left_of_min_cut(int a) { return height[a] >= (int)g.size() - 1; }
};
