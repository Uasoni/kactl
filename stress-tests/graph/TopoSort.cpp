#include "../utilities/template.h"
#include "../utilities/random.h"

#include "../../content/graph/TopoSort.h"

int main() {
	for (int it = 0; it < (50000); ++it) {
		int n = rand() % 20;
		int m = n ? rand() % 30 : 0;
		bool acyclic = rand_bool();
		vector<int> order(n);
		iota(begin(order), end(order), 0);
		shuffle_vec(order);
		vector<vector<int>> ed(n);
		for (int i = 0; i < (m); ++i) {
			int a = rand() % n;
			int b = rand() % n;
			if (acyclic && a >= b) continue;
			ed[order[a]].push_back(order[b]);
		}
		vector<int> ret = topo_sort(ed);
		if (acyclic) assert((int)(ret).size() == n);
		else assert((int)(ret).size() <= n);
		vector<int> seen(n);
		for (int i : ret) {
			assert(!seen[i]++);
			for (int j : ed[i])
				assert(!seen[j]);
		}
	}
	cout << "tests passed!" << endl;
	return 0;
}
