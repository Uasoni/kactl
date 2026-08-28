/**
 * author: unknown, johan sannemo
 * date: 2009-04-17
 * source: tiny_kactl
 * description: old min-cost max-flow. slow, but probably supports negative costs and double edges. returns (flow, cost).
 * status: tested
 */
#pragma once

typedef int flow;
flow inf = 1<<28;

struct FlowEdge {
	int dest, back;
	flow c, f, cost;
	flow r() { return c - f; }
	FlowEdge(int d, int b, flow c, flow cost = 0)
		: dest(d), back(b), c(c), f(0), cost(cost) {}
};

template<class G>
void flow_add_edge(G& g, int s, int t,
		flow c, flow cost = 0) {
	assert(s != t);
	g[s].push_back(FlowEdge(t, (int)(g[t]).size(), c, cost));
	g[t].push_back(FlowEdge(s, (int)(g[s]).size() - 1, 0, cost));
}

template<class G>
pair<flow, flow> aug(G &g, int s, int t) {
	int n = (int)(g).size();
	vector<int> mark(n, -1);
	vector<flow> mindist(n, inf);
	bool changed = true; mindist[s] = 0;
	for (int i = 1; !(changed = !changed); ++i)
		for (int v = 0; v < n; ++v) if (mindist[v] != inf)
			for(auto &e: g[v]) {
				flow dist = mindist[v] + (e.f<0 ? -e.cost : e.cost);
				if (e.r() > 0 && dist < mindist[e.dest]) {
					if (i >= n) assert(0);// negative cycle! shouldn't be
					mindist[e.dest] = dist;
					mark[e.dest] = e.back;
					changed = true;
				}
			}
	if (mark[t] < 0) return make_pair(0, 0);
	flow inc = inf;
	FlowEdge* e; int v = t;
	while (v != s){
		e = &g[v][mark[v]];
		v = e->dest;
		inc = min(inc, g[v][e->back].r());
	}
	v = t;
	while (v != s)
		e = &g[v][mark[v]], e->f -= inc,
		  v = e->dest, g[v][e->back].f += inc;
	return make_pair(inc, inc * mindist[t]);
}

template<class G>
pair<flow, flow> min_cost_max_flow(G& graph, int s, int t) {
	pair<flow, flow> flow, inc;
	while ((inc = aug(graph, s, t)).first){
		flow.first += inc.first;
		flow.second += inc.second;
	}
	return flow;
}
