#include "../utilities/template.h"
#include "../utilities/genGraph.h"
#include "../utilities/random.h"

#include "../../content/graph/EdgeColoring.h"

void test(int n, const vector<pii>& ed) {
	vector<pii> edges = ed;
	for (pii& edge : edges) ++edge.first, ++edge.second;
	vector<int> deg(n + 1);
	for (pii e : edges) ++deg[e.first], ++deg[e.second];
	int maxdeg = n == 0 ? 0 : *max_element(begin(deg), end(deg));
	vector<int> cols = edge_coloring(n, edges);
	assert((int)(cols).size() == (int)(ed).size());
	vector<vector<bool>> used_cols(n + 1, vector<bool>(maxdeg+1));
	for (int i = 0; i < ((int)(cols).size()); ++i) {
		int col = cols[i];
		assert(cols[i] <= maxdeg);
		for (int x : {edges[i].first, edges[i].second}) {
			assert(!used_cols[x][col]);
			used_cols[x][col] = 1;
		}
	}
}

void test_correct() {
	for (int n = 0; n < (7); ++n) {
		for (int edbits = 0; edbits < ((1 << (n*(n-1)/2))); ++edbits) {
			vector<pii> ed;
			int it = 0;
			for (int i = 0; i < (n); ++i) for (int j = i+1; j < (n); ++j) {
				if (edbits & 1 << (it++)) {
					ed.push_back({i, j});
				}
			}
			if (n <= 4 || n + (int)(ed).size() <= 9) {
				// test all k!*2^k input orders
				sort(begin(ed), end(ed));
				if (n != 0) do {
					for (int bi = 0; bi < ((1 << (int)(ed).size())); ++bi) {
						if (bi) {
							int ind = __builtin_ctz(bi);
							swap(ed[ind].first, ed[ind].second);
						}
						test(n, ed);
					}
				} while (next_permutation(begin(ed), end(ed)));
			} else {
				int its = n == 5 ? 10 : 5;
				for (int it = 0; it < (its); ++it) {
					shuffle_vec(ed);
					for (auto& e : ed) if (rand_bool()) swap(e.first, e.second);
					test(n, ed);
				}
			}
		}
	}
	for (int n = 10; n < (30); ++n) for (int it = 0; it < (200); ++it) {
		int m = rand_incl(n * (n-1) / 2);
		vector<pii> ed = random_simple_graph_as_edge_list(n, m);
		test(n, ed);
	}
	for (int n = 1; n <= 1000000; n *= 2) {
		int m = (int) min(1000000LL / n, (ll)n * (n-1) / 2);
		auto ed = random_simple_graph_as_edge_list(n, m);
		test(n, ed);
	}
	cout << "tests passed!" << endl;
}

void test_perf_random() {
	for (int i = 0; i < (100); ++i) {
		int n = 1000;
		int m = 20000;
		auto ed = random_simple_graph_as_edge_list(n, m);
		edge_coloring(n, ed);
	}
}

void test_perf_regular() {
	int n = 3000;
	int k = 30;
	// m = 45000
	vector<pii> ed = random_regular_graph_as_edge_list(n, k);
	for (int i = 0; i < (100); ++i) edge_coloring(n, ed);
}

int main(int argc, char** argv) {
	srand(2);
	string arg = argc == 1 ? "" : argv[1];
	if (arg == "random") test_perf_random();
	else if (arg == "regular") test_perf_regular();
	else {
		assert(argc == 1);
		test_correct();
	}
}
