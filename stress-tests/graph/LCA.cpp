#include "../utilities/template.h"
#include "../utilities/genTree.h"

#include "../../content/graph/LCA.h"
#include "../../content/graph/BinaryLifting.h"
#include "../../content/data-structures/RMQ.h"

namespace old {

struct LcaReference {
	vector<int> time;
	vector<ll> dist;
	RangeMinimumQuery<pii> range_minimum;

	LcaReference(vector<vector<pii>>& graph) : time((int)graph.size(), -99),
		dist((int)graph.size()), range_minimum(dfs(graph)) {}

	vector<pii> dfs(vector<vector<pii>>& C) {
		vector<tuple<int, int, int, ll>> q = {{1, 0, 0, 0}};
		vector<pii> ret;
		int T = 0, v, p, d; ll di;
		while (!q.empty()) {
			tie(v, p, d, di) = q.back();
			q.pop_back();
			if (d) ret.emplace_back(d, p);
			time[v] = T++;
			dist[v] = di;
			for(auto &e: C[v]) if (e.first != p)
				q.emplace_back(e.first, v, d+1, di + e.second);
		}
		return ret;
	}

	int query(int a, int b) {
		if (a == b) return a;
		a = time[a], b = time[b];
		return range_minimum.query(min(a, b) + 1, max(a, b)).second;
	}
	ll distance(int a, int b) {
		int lca = query(a, b);
		return dist[a] + dist[b] - 2 * dist[lca];
	}
};
}


void get_pars(vector<vector<int>> &tree, int cur, int p, int d, vector<int> &par, vector<int> &depth) {
	par[cur] = p;
	depth[cur] = d;
	for(auto i: tree[cur]) if (i != p) {
		get_pars(tree, i, cur, d+1, par, depth);
	}
}
void test_n(int n, int num) {
	for (int out=0; out<num; out++) {
		auto edges = gen_random_tree(n);
		vector<vector<int>> tree(n + 1);
		vector<vector<pair<int, int>>> old_tree(n + 1);
		for (auto i: edges) {
			int a = i.first + 1, b = i.second + 1;
			tree[a].push_back(b);
			tree[b].push_back(a);
			old_tree[a].push_back({b, 1});
			old_tree[b].push_back({a, 1});
		}
		vector<int> par(n + 1), depth(n + 1);
		get_pars(tree, 1, 1, 0, par, depth);
		vector<vector<int>> tbl = tree_jump(par);
		LcaQuery new_lca(tree);
		old::LcaReference old_lca(old_tree);
		for (int i=0; i<100; i++) {
			int a = rand() % n + 1, b = rand() % n + 1;
			int bin_lca = lca(tbl, depth, a, b);
			int new_answer = new_lca.query(a, b);
			int old_answer = old_lca.query(a, b);
			assert(old_answer == new_answer);
			assert(bin_lca == new_answer);
		}
	}
}

signed main() {
	test_n(10, 1000);
	test_n(100, 100);
	test_n(1000, 10);
	cout<<"tests passed!"<<endl;
}
