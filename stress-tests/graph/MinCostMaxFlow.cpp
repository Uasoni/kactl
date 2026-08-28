#include "../utilities/template.h"

// #include "mcmf3.h"
// #include "mcmf4.h"
// #include "mcmfold.h"
// #include "mcmfnew.h"
#include <bits/extc++.h>
#define setpi dummy(){} bool setpi
#undef assert
#define assert(x) return x
#include "../../content/graph/MinCostMaxFlow.h"
#undef assert
#undef setpi
#include "MinCostMaxFlow2.h"

struct MinCostMaxFlowReference {
	vector<vector<FlowEdge>> g;
	MinCostMaxFlowReference(int n) : g(n + 1) {}
	void add_edge(int s, int t, flow c, flow cost = 0) {
		flow_add_edge(g, s, t, c, cost);
	}
	pair<ll, ll> maxflow(int s, int t) {
		return min_cost_max_flow(g, s, t);
	}
	void setpi(int s) {}
};

// typedef MinCostMaxFlowReference MinCostMaxFlowGraph;

#if 1
static size_t i;
#else
static char buf[450 << 20];
static size_t i = sizeof buf;
void* operator new(size_t s) {
	assert(s < i);
	return (void*)&buf[i -= s];
}
void operator delete(void*) noexcept {}
#endif
bool zero(ll x) { return x == 0; }
ll min_cost_matching(const vector<vector<ll>>& cost, vector<int>& L, vector<int>& R) {
	int n = (int)(cost).size(), mated = 0;
	vector<ll> dist(n), u(n), v(n);
	vector<int> dad(n), seen(n);

	/// construct dual feasible solution
	for (int i = 0; i < (n); ++i) {
		u[i] = cost[i][0];
		for (int j = 1; j < (n); ++j) u[i] = min(u[i], cost[i][j]);
	}
	for (int j = 0; j < (n); ++j) {
		v[j] = cost[0][j] - u[0];
		for (int i = 1; i < (n); ++i) v[j] = min(v[j], cost[i][j] - u[i]);
	}

	/// find primal solution satisfying complementary slackness
	L = vector<int>(n, -1);
	R = vector<int>(n, -1);
	for (int i = 0; i < (n); ++i) for (int j = 0; j < (n); ++j) {
		if (R[j] != -1) continue;
		if (zero(cost[i][j] - u[i] - v[j])) {
			L[i] = j;
			R[j] = i;
			mated++;
			break;
		}
	}

	for (; mated < n; mated++) { // until solution is feasible
		int s = 0;
		while (L[s] != -1) s++;
		fill(begin(dad), end(dad), -1);
		fill(begin(seen), end(seen), 0);
		for (int k = 0; k < (n); ++k)
			dist[k] = cost[s][k] - u[s] - v[k];

		int j = 0;
		for (;;) { /// find closest
			j = -1;
			for (int k = 0; k < (n); ++k){
				if (seen[k]) continue;
				if (j == -1 || dist[k] < dist[j]) j = k;
			}
			seen[j] = 1;
			int i = R[j];
			if (i == -1) break;
			for (int k = 0; k < (n); ++k) { /// relax neighbors
				if (seen[k]) continue;
				auto new_dist = dist[j] + cost[i][k] - u[i] - v[k];
				if (dist[k] > new_dist) {
					dist[k] = new_dist;
					dad[k] = j;
				}
			}
		}

		/// update dual variables
		for (int k = 0; k < (n); ++k) {
			if (k == j || !seen[k]) continue;
			auto w = dist[k] - dist[j];
			v[k] += w, u[R[k]] -= w;
		}
		u[s] += dist[j];

		/// augment along path
		while (dad[j] >= 0) {
			int d = dad[j];
			R[j] = R[d];
			L[R[j]] = j;
			j = d;
		}
		R[j] = s;
		L[s] = j;
	}

	auto value = vector<ll>(1)[0];
	for (int i = 0; i < (n); ++i) value += cost[i][L[i]];
	return value;
}

void test_perf() {
	srand(2);
	int n = 500, edge_count = 10000, max_capacity = 100, max_cost = 100000;
	MinCostMaxFlowGraph mcmf(n);
	int s = 1, t = 2;
	for (int i = 0; i < edge_count; ++i) {
		int a = rand() % n + 1;
		int b = rand() % n + 1;
		int cap = rand() % max_capacity;
		int cost = rand() % max_cost;
		if (a == b) continue;
		mcmf.add_edge(a, b, cap, cost);
		// ::cap[a][b] = cap;
		// ::cost[a][b] = cost;
	}
	auto pa = mcmf.maxflow(s, t);
	cout << pa.first << ' ' << pa.second << endl;
}

void test_matching() {
	for (int it = 0; it < (100000); ++it) {
		size_t last = ::i;
		int n = rand() % 10, m = rand() % 10;
		int square_size = max(n, m);
		vector<vector<ll>> co(square_size, vector<ll>(square_size));
		for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) co[i][j] = (rand() % 10) + 2;
		vector<int> left_match, right_match;
		ll v = min_cost_matching(co, left_match, right_match);
		int source = n + m + 1, sink = n + m + 2;
		MinCostMaxFlowGraph mcmf(n + m + 2);
		for (int i = 1; i <= n; ++i) mcmf.add_edge(source, i, 1, 0);
		for (int j = 1; j <= m; ++j) mcmf.add_edge(n + j, sink, 1, 0);
		for (int i = 1; i <= n; ++i) for (int j = 1; j <= m; ++j)
			mcmf.add_edge(i, n + j, 1, co[i - 1][j - 1] - 2);
		mcmf.setpi(source);
		auto pa = mcmf.maxflow(source, sink);
		assert(pa.first == min(n, m));
		assert(pa.second == v - 2 * pa.first);
		::i = last;
	}
}

void test_neg() {
	const int ITERATIONS = 1000000;
	int ed[100][100];
	int negs = 0;
	for (int it = 0; it < ITERATIONS; ++it) {
		size_t lasti = ::i;
		int n = rand() % 7 + 2;
		int edge_count = rand() % 17;
		int source = 1, sink = 2;
		MinCostMaxFlowGraph mcmf(n);
		MinCostMaxFlowReference mcmf2(n);
		for (int i = 1; i <= n; ++i) for (int j = 1; j <= n; ++j) ed[i][j] = 0;
		bool anyneg = false;
		for (int eid = 0; eid < edge_count; ++eid) {
			int i = rand() % n + 1, j = rand() % n + 1;
			if (i != j && !ed[i][j]) {
				ed[i][j] = 1;
				int fl = rand() % 50;
				int co = rand() % 11 - 3;
				mcmf.add_edge(i, j, fl, co);
				mcmf2.add_edge(i, j, fl, co);
				if (co < 0) anyneg = true;
			}
		}
		negs += anyneg;
		if (anyneg || rand() % 3 == 0) {
			if (!mcmf.setpi(source))  // has negative loops
				continue;
		}
		auto pa = mcmf.maxflow(source, sink);
		auto pa2 = mcmf2.maxflow(source, sink);
		assert(pa == pa2);
		::i = lasti;
	}
	// cerr << negs << "/" << ITS << " graphs with negative edge costs" << endl;
	cout<<"tests passed!"<<endl;
}

int main() {
	test_matching();
	test_neg();
}
