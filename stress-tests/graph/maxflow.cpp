#include "../utilities/template.h"

#include "../../content/graph/PushRelabel.h"
#include "../../content/graph/Dinic.h"
#include "../../content/graph/EdmondsKarp.h"

// bump allocator, to speed the test up and get rid of malloc performance noise
static char buf[1 << 23];
static size_t bufi = sizeof buf;
void* operator new(size_t s) {
	assert(s < bufi);
	return (void*)&buf[bufi -= s];
}
void operator delete(void*) {}

int main() {
	for (int it = 0; it < (500000); ++it) {
		bufi = sizeof buf;
		int n = 2 + rand() % 10;
		int s = rand() % n + 1;
		int t = rand() % (n - 1) + 1;
		if (t >= s) t++;
		PushRelabel pr(n);
		Dinic dinic(n);
		vector<unordered_map<int, int>> ek(n + 1);

		int m = rand() % 40;
		for (int eit = 0; eit < (m); ++eit) {
			int a = rand() % n + 1;
			int b = rand() % n + 1;
			int c = rand() % 4;
			int d = rand() % 4 == 0 ? rand() % 3 + 1 : 0;
			pr.add_edge(a, b, c, d);
			dinic.add_edge(a, b, c, d);
			ek[a][b] += c;
			ek[b][a] += d;
		}
		auto orig_ek = ek;

		ll flow = pr.calc(s, t);

		// PushRelabel matches Dinic
		ll dinic_flow = dinic.calc(s, t);
		assert(flow == dinic_flow);

		// PushRelabel matches edmonds_karp
		ll ek_flow = edmonds_karp(ek, s, t);
		assert(flow == ek_flow);

		// conservation of flow for PushRelabel
		vector<ll> flows(n + 1);
		for (int i = 1; i <= n; ++i) for(auto &e: pr.g[i]) if (e.f > 0) {
			assert(e.c >= 0);
			flows[i] += e.f;
			flows[e.dest] -= e.f;
		}
		assert(flow == flows[s]);
		assert(flow == -flows[t]);
		for (int i = 1; i <= n; ++i) if (i != s && i != t) assert(flows[i] == 0);

		// conservation of flow for Dinic
		vector<ll> dinic_flows(n + 1);
		for (int i = 1; i <= n; ++i) for(auto &e: dinic.adj[i]) {
			assert(e.flow() <= e.oc);
			dinic_flows[i] += e.flow();
			dinic_flows[e.to] -= e.flow();
		}
		assert(flows == dinic_flows);

		// conservation of flow for edmonds_karp
		vector<ll> ek_flows(n + 1);
		for (int i = 1; i <= n; ++i) for(auto &e: orig_ek[i]) {
			int nc = ek[i][e.first];
			assert(nc >= 0);
			int flow = e.second - nc;
			if (flow > 0) {
				ek_flows[i] += flow;
				ek_flows[e.first] -= flow;
			}
		}
		assert(flows == ek_flows);

		// PushRelabel computes a cut
		ll across_cut = 0;
		assert(pr.left_of_min_cut(s));
		assert(!pr.left_of_min_cut(t));
		for (int i = 1; i <= n; ++i) for(auto &e: pr.g[i]) {
			if (pr.left_of_min_cut(i) && !pr.left_of_min_cut(e.dest)) {
				assert(e.f >= 0);
				assert(e.c == 0);
				across_cut += e.f;
			}
			if (!pr.left_of_min_cut(i) && pr.left_of_min_cut(e.dest)) {
				assert(e.f <= 0);
			}
		}

		// min cut = max flow
		assert(across_cut == flow);
	}

	cout << "tests passed!" << endl;
}
