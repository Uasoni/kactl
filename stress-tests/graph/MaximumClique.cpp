#include "../utilities/template.h"

#include "../../content/graph/MaximumClique.h"
namespace maximal {
#include "../../content/graph/MaximalCliques.h"
}

struct StressTimer {
	decltype(chrono::high_resolution_clock::now()) begin;
	const string label;
	StressTimer(string label = "???") : label(label) { begin = chrono::high_resolution_clock::now(); }
	~StressTimer() {
		auto end = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
		cerr << duration << "ms elapsed [" << label << "]" << endl;
	}
};


int main() {
	for (int it = 0; it < (100000); ++it) {
		int n =(rand()%32)+1;
		vector<bitset<200>> ed(n + 1);
		vector<bitset<128>> ed2(n + 1);
		int p =rand()%100;
		for (int i = 1; i <= n; ++i) for (int j = 1; j < i; ++j) {
			ed[i][j] = (rand() % 100) < p;
			ed[j][i] = ed[i][j];
			ed2[i][j] = ed[i][j];
			ed2[j][i] = ed[j][i];
		}
		MaximumClique clique2(ed);
		int mx = 0;
		maximal::cliques(ed2, [&](auto x){mx = max(mx, int(x.count()));});
		assert(mx == (int)(clique2.max_clique()).size());
	}
	cout<<"tests passed!"<<endl;
}
