#include "../utilities/template.h"

#include "../../content/graph/EulerWalk.h"

struct UnionFind {
	vector<int> v;
	UnionFind(int n) : v(n, -1) {}
	int find(int x) { return v[x] < 0 ? x : v[x] = find(v[x]); }
	void join(int a, int b) {
		a = find(a);
		b = find(b);
		if (a == b) return;
		if (-v[a] < -v[b]) swap(a, b);
		v[a] += v[b];
		v[b] = a;
	}
};

bool has_euler_walk(vector<vector<pii>>& ed, int start, bool undir, bool cycle) {
	int n = (int)ed.size() - 1;
	int odd = 0;
	bool any_edges = false;
	vector<int> nins(n + 1);
	for (int i = 1; i <= n; ++i) {
		for(auto &x: ed[i]) nins[x.first]++;
	}
	for (int i = 1; i <= n; ++i) {
		if (!ed[i].empty()) any_edges = true;
		if (undir) {
			assert((int)(ed[i]).size() == nins[i]);
			int nout = 0;
			for(auto &x: ed[i]) if (x.first != i) nout++;
			if (i != start && nout % 2) odd++;
		}
		else {
			if (nins[i] == (int)(ed[i]).size()) continue;
			if (cycle) return false;
			if (abs(nins[i] - (int)(ed[i]).size()) > 1) { return false; }
			if (nins[i] < (int)(ed[i]).size() && i != start) { return false; }
		}
	}
	if (odd > !cycle) { return false; }
	if (ed[start].empty() && any_edges) { return false; }
	UnionFind uf(n + 1);
	for (int i = 1; i <= n; ++i) for(auto &x: ed[i]) uf.join(i, x.first);
	int comp = 0;
	for (int i = 1; i <= n; ++i) if (uf.find(i) == i) {
		if (ed[i].empty()) continue;
		comp++;
	}
	return comp <= 1;
}

vector<int> euler_cycle(vector<vector<pii>>& gr, int nedges, int src=1) {
	int n = (int)(gr).size();
	vector<int> D(n), its(n), eu(nedges), ret, s = {src};
	// D[src]++; // to allow euler paths, not just cycles
	while (!s.empty()) {
		int x = s.back(), y, e, &it = its[x], end = (int)(gr[x]).size();
		if (it == end){ ret.push_back(x); s.pop_back(); continue; }
		tie(y, e) = gr[x][it++];
		if (!eu[e]) {
			D[x]--, D[y]++;
			eu[e] = 1; s.push_back(y);
		}}
	for(auto &x: D) if (x < 0 || (int)(ret).size() != nedges+1) return {};
	return {ret.rbegin(), ret.rend()};
}

int main() {
	for (int cycle = 0; cycle < (2); ++cycle) for (int undir = 0; undir < (2); ++undir) {
		for (int it = 0; it < (10000); ++it) {
			int n = rand() % 10 + 1;
			int m = rand() % 20;
			int start = rand() % n + 1;
			vector<vector<pii>> ed(n + 1);
			map<pii, vector<int>> all_eds;
			vector<pii> the_edges;
			for (int i = 0; i < (m); ++i) {
				int a = rand() % n + 1;
				int b = rand() % n + 1;
				ed[a].emplace_back(b, i);
				all_eds[pii(a, b)].push_back(i);
				if (undir) {
					ed[b].emplace_back(a, i);
					all_eds[pii(b, a)].push_back(i);
				}
				the_edges.emplace_back(a, b);
			}

			vector<int> res = cycle ? euler_cycle(ed, m, start) : euler_walk(ed, m, start);
			if (0) {
				cout << n << ' ' << m << ' ' << start << ' ' << undir << ' ' << cycle << endl;
				for (int i = 1; i <= n; ++i) {
					for(auto &x: ed[i]) cout << x.first << ' ';
					cout << endl;
				}
				cout << "returned" << endl;
				for(auto &x: res) cout << x << ' ';
				cout << endl;
				cout << "of length " << (int)(res).size() << endl;
			}

			if (res.empty()) {
				assert(!has_euler_walk(ed, start, undir, cycle));
			} else {
				assert(has_euler_walk(ed, start, undir, cycle));

				assert(res[0] == start);
				if (cycle) assert(res.back() == start);
				int cur = start;
				vector<int> seen_edge(m);
				for (int i = 1; i < ((int)(res).size()); ++i) {
					int x = res[i];
					for(auto &eid: all_eds[pii(cur, x)]) {
						if (!seen_edge[eid]) {
							seen_edge[eid] = 1;
							goto ok;
						}
					}
					assert(0); // no usable edge there
ok:
					cur = x;
				}
			}
		}
	}
	cout << "tests passed!" << endl;
}
