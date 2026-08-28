#include "../utilities/template.h"

#include "../../content/graph/MaximalCliques.h"

template<class F>
void fast_cliques(vector<bitset<128>>& eds, F f) {
	bitset<128> current{}, candidates = ~bitset<128>(), excluded{};
	vector<int> deg((int)(eds).size());
	for (int i = 0; i < ((int)(eds).size()); ++i) deg[i] = (int)(eds[i]).size();
	for (int j = 0; j < ((int)(eds).size()); ++j) {
		int i = (int)(min_element(begin(deg), end(deg)) - deg.begin());
		current[i] = 1;
		rec(eds, current, candidates & eds[i], excluded & eds[i], f);
		current[i] = candidates[i] = 0; excluded[i] = 1;
		for (int k = 0; k < ((int)(eds).size()); ++k) if (eds[i][k]) deg[k]--;
		deg[i] = 1000000;
	}
}

int test_correctness() {
	for (int n = 1; n < (11); ++n) for (int m = 0; m < (200); ++m) {
		vector<bitset<128>> ed(n + 1);
		for (int i = 0; i < (m); ++i) {
			int a = rand() % n + 1, b = rand() % n + 1;
			if (a == b) continue;
			ed[a][b] = 1;
			ed[b][a] = 1;
		}
		unordered_set<bitset<128>> cl;
		int co = 0;
		cliques(ed, [&](bitset<128> x) {
			co++;
			cl.insert(x);
		});
		assert((int)(cl).size() == co); // no duplicates
		auto is_clique = [&](bitset<128> c) {
			for (int i = 1; i <= n; ++i) if (c[i])
			for (int j = i + 1; j <= n; ++j) if (c[j]) {
				if (!ed[i][j]) return false; // not a clique
			}
			for (int i = 1; i <= n; ++i) if (!c[i]) {
				bool all = true;
				for (int j = 1; j <= n; ++j) if (c[j]) all &= ed[i][j];
				if (all) return false; // not maximal
			}
			return true;
		};
		for(auto &c: cl) {
			assert(is_clique(c));
		}

		int real_co = 0;
		for (int bi = 0; bi < ((1 << n)); ++bi) {
			bitset<128> c{};
			for (int i = 1; i <= n; ++i) c[i] = !!(bi & (1 << (i - 1)));
			if (is_clique(c)) real_co++;
		}
		assert(co == real_co);
	}
	cout<<"tests passed!"<<endl;
	return 0;
}

int test_performance() {
	for (int it = 0; it < (20); ++it) {
		const int n = 127, m = 4000;
		vector<bitset<128>> ed(n + 1);
		for (int i = 0; i < (m); ++i) {
			int a = rand() % n + 1, b = rand() % n + 1;
			if (a == b) continue;
			ed[a][b] = 1;
			ed[b][a] = 1;
		}
		int co = 0, sum = 0;
		cliques(ed, [&](bitset<128> x) { co++; sum += (int)x.count(); });
		cout << co << ' ' << (double)sum / co << endl;
	}
	return 0;
}

int main(int argc, char**) {
	return argc > 1 ? test_performance() : test_correctness();
}
