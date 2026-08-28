/**
 * Author: stanford
 * Date: unknown
 * Source: stanford notebook
 * Description: min-cost max-flow.
 *  if costs can be negative, call set\_pi before max\_flow, but note that negative cost cycles are not supported.
 *  to obtain the actual flow, look at positive values only.
 * vertices are numbered $1..n$.
 * Status: tested on kattis:mincostmaxflow, stress-tested against another implementation
 * Time: $O(F E \log(V))$ where F is max flow. $O(VE)$ for set\_pi.
 */
#pragma once

const ll INF = numeric_limits<ll>::max() / 4;

struct MinCostMaxFlowGraph {
	struct Edge {
		int from, to, rev;
		ll cap, cost, flow;
	};
	int vertex_count;
	vector<vector<Edge>> ed;
	vector<int> seen;
	vector<ll> dist, pi;
	vector<Edge*> par;

	MinCostMaxFlowGraph(int n) : vertex_count(n), ed(n + 1), seen(n + 1),
		dist(n + 1), pi(n + 1), par(n + 1) {}

	void add_edge(int from, int to, ll cap, ll cost) {
		if (from == to) return;
		ed[from].push_back(Edge{ from,to,(int)(ed[to]).size(),cap,cost,0 });
		ed[to].push_back(Edge{ to,from,(int)(ed[from]).size()-1,0,-cost,0 });
	}

	void shortest_path(int s) {
		fill(begin(seen), end(seen), 0);
		fill(begin(dist), end(dist), INF);
		fill(begin(par), end(par), nullptr);
		dist[s] = 0; ll di;

		priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> q;
		q.push({ 0, s });

		while (!q.empty()) {
			s = q.top().second; q.pop();
			if (seen[s]) continue;
			seen[s] = 1; di = dist[s] + pi[s];
			for (Edge& e : ed[s]) if (!seen[e.to]) {
				ll val = di - pi[e.to] + e.cost;
				if (e.cap - e.flow > 0 && val < dist[e.to]) {
					dist[e.to] = val;
					par[e.to] = &e;
					q.push({dist[e.to], e.to});
				}
			}
		}
		for (int i = 1; i <= vertex_count; ++i) pi[i] = min(pi[i] + dist[i], INF);
	}

	pair<ll, ll> max_flow(int s, int t) {
		ll total_flow = 0, total_cost = 0;
		while (shortest_path(s), seen[t]) {
			ll fl = INF;
			for (Edge* x = par[t]; x; x = par[x->from])
				fl = min(fl, x->cap - x->flow);

			total_flow += fl;
			for (Edge* x = par[t]; x; x = par[x->from]) {
				x->flow += fl;
				ed[x->to][x->rev].flow -= fl;
			}
		}
		for (int i = 1; i <= vertex_count; ++i)
			for (Edge& e : ed[i]) total_cost += e.cost * e.flow;
		return {total_flow, total_cost/2};
	}

	// if some costs can be negative, call this before max_flow:
	void set_pi(int s) { // (otherwise, leave this out)
		fill(begin(pi), end(pi), INF); pi[s] = 0;
		int it = vertex_count, ch = 1; ll v;
		while (ch-- && it--)
			for (int i = 1; i <= vertex_count; ++i) if (pi[i] != INF)
			  for (Edge& e : ed[i]) if (e.cap)
				  if ((v = pi[i] + e.cost) < pi[e.to])
					  pi[e.to] = v, ch = 1;
		assert(it >= 0); // negative cost cycle
	}
};
