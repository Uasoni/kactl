#include "../utilities/template.h"
#include "../utilities/genTree.h"

#include "../../content/graph/HLD.h"

namespace old {
#include "oldHLD.h"
}
struct BruteForce { // values in nodes
	vector<vector<int>> tree;
	vector<int> vals;
	vector<int> pars;
	int unit = -1e9;
	int f(int a, int b) { return max(a, b); }
	void root(int cur, int p = -1) {
		pars[cur] = p;
		for (auto i: tree[cur]) {
			if (i != p) root(i, cur);
		}
	}
	BruteForce(vector<vector<int>> _tree): tree(_tree), vals((int)(tree).size()), pars((int)(tree).size()) {
		root(1, 0);
	}
	bool dfs_modify(int cur, int target, int val, int p=-1) {
		if (cur == target) {
			vals[cur] += val;
			return true;
		}
		bool along_path = false;
		for (auto i: tree[cur]) {
			if (i == p) continue;
			along_path |= dfs_modify(i, target, val, cur);
		}
		if (along_path) vals[cur] += val;
		return along_path;
	}
	void modify_path(int a, int b, int val) {
		dfs_modify(a, b, val);
	}

	int dfs_query(int cur, int target, int p = -1) {
		if (cur == target) {
			return vals[cur];
		}
		int res = unit;
		for (auto i: tree[cur]) {
			if (i == p) continue;
			res = f(res, dfs_query(i, target, cur));
		}
		if (res != unit) {
			return f(res, vals[cur]);
		}
		return res;
	}
	int query_path(int a, int b) {
		return dfs_query(a, b);
	}
	int dfs_subtree(int cur, int p) {
		int res = vals[cur];
		for (auto i: tree[cur]) {
			if (i != p)
				res = f(res, dfs_subtree(i, cur));
		}
		return res;
	}
	int query_subtree(int a) {
		return dfs_subtree(a, pars[a]);
	}
};

void test_against_old(int n, int iters, int queries) {
	for (int trees = 0; trees < iters; trees++) {
		auto graph = gen_random_tree(n);
		vector<vector<int>> tree1(n + 1);
		vector<vector<pair<int, int>>> tree2(n + 1);
		for (auto i : graph) {
			int a = i.first + 1, b = i.second + 1;
			tree1[a].push_back(b);
			tree1[b].push_back(a);
		}
		for (int i = 1; i <= n; i++) {
			for (auto j : tree1[i]) {
				tree2[i].push_back({j, 0});
			}
		}
		HeavyLightDecomposition<false> hld(tree1);
		old::HeavyLightDecomposition hld2(tree2);
		hld.tree->set(1, n, 0);
		for (int itr = 0; itr < queries; itr++) {
			if (rand() % 2) {
				int node = rand() % n + 1;
				int val = rand() % 10;
				hld2.update(node, val);
				hld.modify_path(node, node, val - hld.query_path(node, node));
			} else {
				int a = rand() % n + 1;
				int b = rand() % n + 1;
				assert(hld.query_path(a, b) == hld2.query2(a, b).first);
			}
		}
	}
}
void test_against_brute(int n, int iters, int queries) {
	for (int trees = 0; trees < iters; trees++) {
		auto graph = gen_random_tree(n);
		vector<vector<int>> tree1(n + 1);
		for (auto i : graph) {
			int a = i.first + 1, b = i.second + 1;
			tree1[a].push_back(b);
			tree1[b].push_back(a);
		}
		HeavyLightDecomposition<false> hld(tree1);
		BruteForce hld2(tree1);
		hld.tree->set(1, n, 0);
		for (int itr = 0; itr < queries; itr++) {
			int rng = rand() % 3;
			if (rng == 0) {
				int a = rand() % n + 1;
				int b = rand() % n + 1;
				int val = rand() % 10;
				hld.modify_path(a, b, val);
				hld2.modify_path(a, b, val);
			} else if (rng == 1){
				int a = rand() % n + 1;
				int b = rand() % n + 1;
				hld.query_path(a, b);
				hld2.query_path(a, b);
				assert(hld.query_path(a, b) == hld2.query_path(a, b));
			} else if (rng == 2) {
				int a = rand() % n + 1;
				assert(hld.query_subtree(a) == hld2.query_subtree(a));
			}
		}
	}

}
int main() {
	srand(2);
	test_against_brute(5, 1000, 10000);
	test_against_brute(1000, 100, 100);
	test_against_old(5, 1000, 100);
	test_against_old(10000, 100, 1000);
	cout<<"tests passed!"<<endl;
	return 0;
}
