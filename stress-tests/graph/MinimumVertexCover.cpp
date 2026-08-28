#include "../utilities/template.h"

#include "../../content/graph/MinimumVertexCover.h"
#include "../../content/graph/HopcroftKarp.h"

vector<int> cover_hk(vector<vector<int>>& g, int n, int m) {
	vector<int> match(m + 1, -1);
	int res = hopcroft_karp(g, match);
	vector<bool> lfound(n + 1, true), seen(m + 1);
	for (int i = 1; i <= m; ++i) if (match[i] != -1) lfound[match[i]] = false;
	vector<int> q, cover;
	for (int i = 1; i <= n; ++i) if (lfound[i]) q.push_back(i);
	while (!q.empty()) {
		int i = q.back(); q.pop_back();
		lfound[i] = 1;
		for(auto &e: g[i]) if (!seen[e] && match[e] != -1) {
			seen[e] = true;
			q.push_back(match[e]);
		}
	}
	for (int i = 1; i <= n; ++i) if (!lfound[i]) cover.push_back(i);
	for (int i = 1; i <= m; ++i) if (seen[i]) cover.push_back(n+i);
	assert((int)(cover).size() == res);
	return cover;
}

int main() {
	const int MAX_N = 20;
	mt19937 rng(17);
	for (int it = 0; it < (300'000); ++it) {
		int n = rand() % MAX_N, m = rand() % MAX_N;
		vector<vector<int>> gr(n + 1);
		int co = rand() % 128 < 10 ? 2 : 1;
		for (int _ = 0; _ < (co); ++_) {
			int prop = rand();
			if (max(n, m) >= 5 && rand() % 128 < 90) {
				prop /= max(n, m);
				prop *= 2;
			}
			for (int i = 1; i <= n; ++i) for (int j = 1; j <= m; ++j) if (rand() < prop) {
				gr[i].push_back(j);
			}
		}
		if (rand() % 128 < 20 && n && m) {
			int paths = rand() % min(n, m);
			vector<int> left(n), right(m);
			for (int i = 0; i < n; ++i) left[i] = i + 1;
			for (int i = 0; i < m; ++i) right[i] = i + 1;
			for (int _ = 0; _ < (paths); ++_) {
				shuffle(begin(left), end(left), rng);
				shuffle(begin(right), end(right), rng);
				int len = rand() % min(n, m) + 1;
				if (rand() & 128) gr[left[0]].push_back(right[0]);
				for (int i = 1; i < (len-1); ++i) {
					gr[left[i]].push_back(right[i-1]);
					gr[left[i]].push_back(right[i]);
				}
				if (len > 1 && (rand() & 128)) gr[left.back()].push_back(right.back());
			}
		}
		for (int i = 1; i <= n; ++i) {
			// duplicate edges are okay
			// sort(begin(gr[i]), end(gr[i]));
			// gr[i].erase(unique(begin(gr[i]), end(gr[i])), gr[i].end());
			shuffle(begin(gr[i]), end(gr[i]), rng);
		}
		auto verify = [&](vector<int>& cover) {
			vector<int> left(n + 1), right(m + 1);
			for(auto &x: cover) {
				if (x <= n) left[x] = 1;
				else right[x - n] = 1;
			}
			for (int i = 1; i <= n; ++i) if (!left[i]) for(auto &j:gr[i]) {
				assert(right[j]);
				/* if (!right[j]) {
					cout << n << ' ' << m << endl;
					for (int i = 0; i < (n); ++i) for(auto &j: gr[i]) cout << i << " - " << j << endl;
					cout << "yields " << (int)(cover).size() << endl;
					for(auto &x: cover) cout << x << endl;
					abort();
				} */
			}
		};
		vector<int> cover1 = cover(gr, n, m);
		vector<int> cover2 = cover_hk(gr, n, m);
		assert((int)(cover1).size() == (int)(cover2).size());
		verify(cover1);
		verify(cover2);
		// cout << '.' << endl;
	}
	cout<<"tests passed!"<<endl;
}
