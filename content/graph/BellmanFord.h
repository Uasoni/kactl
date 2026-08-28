/**
 * Author: simon lindholm
 * Date: 2015-02-23
 * License: CC0
 * Source: http://en.wikipedia.org/wiki/bellman-ford_algorithm
 * Description: calculates shortest paths from $s$ in a graph that might have negative edge weights.
 * unreachable nodes get dist = inf; nodes reachable through negative-weight cycles get dist = -inf.
 * assumes $V^2 \max |w_i| < \tilde{} 2^{63}$. Vertices are $1..n$ and the
 * node vector has size $n+1$.
 * Time: O(VE)
 * Status: tested on kattis:shortestpath3
 */
#pragma once

const ll INF = LLONG_MAX;
struct BellmanEdge { int a, b, w, s() { return a < b ? a : -a; }};
struct BellmanNode { ll dist = INF; int prev = -1; };

void bellman_ford(vector<BellmanNode>& nodes, vector<BellmanEdge>& eds, int s) {
	nodes[s].dist = 0;
	sort(begin(eds), end(eds), [](BellmanEdge a, BellmanEdge b) { return a.s() < b.s(); });

	int lim = (int)(nodes).size() / 2 + 2; // /3+100 with shuffled vertices
	for (int i = 0; i < lim; ++i) for (BellmanEdge edge : eds) {
		BellmanNode cur = nodes[edge.a], &dest = nodes[edge.b];
		if (abs(cur.dist) == INF) continue;
		ll d = cur.dist + edge.w;
		if (d < dest.dist) {
			dest.prev = edge.a;
			dest.dist = (i < lim-1 ? d : -INF);
		}
	}
	for (int i = 0; i < lim; ++i) for (BellmanEdge e : eds) {
		if (nodes[e.a].dist == -INF)
			nodes[e.b].dist = -INF;
	}
}
