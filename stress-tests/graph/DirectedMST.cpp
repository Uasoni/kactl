#include "../utilities/template.h"

struct BumpAllocator {
	char buf[450 << 20];
	size_t bufp;
	void* alloc(size_t s) {
		assert(s < bufp);
		return (void*)&buf[bufp -= s];
	}
	BumpAllocator() { reset(); }

	template<class T> T* operator=(T&& x) {
		T* r = (T*)alloc(sizeof(T));
		new(r) T(move(x));
		return r;
	}
	void reset() { bufp = sizeof buf; }
} BumpAllocator;

// when not testing perf, we don't want to leak memory
#ifndef TEST_PERF
#define new BumpAllocator =
#endif
#include "../../content/graph/DirectedMST.h"
#ifndef TEST_PERF
#undef new
#endif

namespace mit {

constexpr int MAX_VERTICES = 110'000;
constexpr int MAX_EDGES = 110'000;
constexpr int INF = 2'000'000'000;

struct DirectedEdge {
		int u, v;
		int cost;
} edges[MAX_EDGES], edge_copy[MAX_EDGES];

int incoming[MAX_VERTICES], component_id[MAX_VERTICES];
int visited[MAX_VERTICES], previous[MAX_VERTICES];

// edges pointed from root.
int directed_mst(int root, int vertex_count, int edge_count) {
	for (int i = 0; i < edge_count; i++)
		edge_copy[i] = edges[i];
	int ret = 0;
	int u, v;
	while (true) {
		for (int i = 0; i < vertex_count; ++i) incoming[i] = INF;
		for (int i = 0; i < (edge_count); ++i) {
			u = edge_copy[i].u;
			v = edge_copy[i].v;
			if(edge_copy[i].cost < incoming[v] && u != v) {
				incoming[v] = edge_copy[i].cost;
				previous[v] = u;
			}
		}
		for (int i = 0; i < (vertex_count); ++i) {
			if(i == root)   continue;
			if(incoming[i] == INF) return -1; // no solution
		}

		int cnt = 0;
		for (int i = 0; i < (vertex_count); ++i) {
			component_id[i] = -1;
			visited[i] = -1;
		}
		incoming[root] = 0;

		for (int i = 0; i < (vertex_count); ++i) {
			ret += incoming[i];
			int v = i;
			while(visited[v] != i && component_id[v] == -1 && v != root) {
				visited[v] = i;
				v = previous[v];
			}
			if(v != root && component_id[v] == -1) {
				for(u = previous[v]; u != v; u = previous[u]) {
					component_id[u] = cnt;
				}
				component_id[v] = cnt++;
			}
		}
		if(cnt == 0)    break;
		for (int i = 0; i < (vertex_count); ++i) {
			if(component_id[i] == -1) component_id[i] = cnt++;
		}
		for (int i = 0; i < (edge_count); ++i) {
			v = edge_copy[i].v;
			edge_copy[i].u = component_id[edge_copy[i].u];
			edge_copy[i].v = component_id[edge_copy[i].v];
			if(edge_copy[i].u != edge_copy[i].v) {
				edge_copy[i].cost -= incoming[v];
			}
		}
		vertex_count = cnt;
		root = component_id[root];
	}
	return ret;
}
}

int adj[105][105];
int main() {
	for (int it = 0; it < (50000); ++it) {
		BumpAllocator.reset();
		int n = (rand()%20)+1;
		int density = rand() % 101;
		int r = rand()%n;
		int cnt = 0;
		vector<Edge> edges;
		for (int i = 0; i < (n); ++i)
			for (int j = 0; j < (n); ++j){
				if (i==j) continue;
				if (rand() % 100 >= density) continue;
				int weight = rand()%100;
				mit::edges[cnt++] = {i,j, weight};
				edges.push_back({i + 1, j + 1, weight});
				adj[i][j] = weight;
			}

		ll ans1 = mit::directed_mst(r, n, cnt);
		auto pa = dmst(n, r + 1, edges);
		ll ans2 = pa.first;
		assert(ans1 == ans2);

		// verifying reconstruction:
		if (ans1 != -1) {
			vector<int> par = pa.second;
			if (0) {
				cout << "r = " << r << endl;
				for(auto &x: par) cout << x << ' ';
				cout << endl;
				for(auto &e: edges) {
					cout << e.a << ' ' << e.b << ' ' << e.w << endl;
				}
			}
			ll sum = 0;
			vector<vector<int>> ch(n);
			for (int i = 0; i < (n); ++i) {
				if (i == r) assert(par[i + 1] == -1);
				else {
					assert(par[i + 1] != -1);
					int parent = par[i + 1] - 1;
					sum += adj[parent][i];
					ch[parent].push_back(i);
				}
			}
			assert(sum == ans1);
			vector<int> seen(n), q = {r};
			for (int qi = 0; qi < ((int)(q).size()); ++qi) {
				int s = q[qi];
				if (!seen[s]++)
					for(auto &x: ch[s]) q.push_back(x);
			}
			assert(count(begin(seen), end(seen), 0) == 0);
		}
	}
	cout<<"tests passed!"<<endl;
	return 0;
}
