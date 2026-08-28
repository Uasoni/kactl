#include "../utilities/template.h"

#include "../../content/graph/LinkCutTree.h"
#include "../../content/data-structures/UnionFind.h"

int main() {
	srand(2);
	LinkCutTree lczero(0);
	for (int it = 0; it < (10000); ++it) {
		int n = rand() % 20 + 1;
		LinkCutTree lc(n);
		UnionFind uf(n);
		vector<pii> edges;
		for (int it2 = 0; it2 < (1000); ++it2) {
			int v = (rand() >> 4) & 3;
			if (v == 0 && !edges.empty()) { // remove
				int r = (rand() >> 4) % (int)(edges).size();
				pii ed = edges[r];
				swap(edges[r], edges.back());
				edges.pop_back();
				if (rand() & 16)
					lc.cut(ed.first, ed.second);
				else
					lc.cut(ed.second, ed.first);
			} else {
				int a = (rand() >> 4) % n + 1;
				int b = (rand() >> 4) % n + 1;
				uf.e.assign(n + 1, -1);
				for(auto &ed: edges) uf.join(ed.first, ed.second);
				bool c = uf.same_set(a, b);
				if (!c && v != 1) {
					lc.link(a, b);
					edges.emplace_back(a, b);
				} else {
					assert(lc.connected(a, b) == c);
				}
			}
		}
	}
	cout<<"tests passed!"<<endl;
}
