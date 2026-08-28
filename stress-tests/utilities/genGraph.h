#pragma once
#include "random.h"

vector<pii> random_simple_graph_as_edge_list(int n, int m) {
	assert(m <= (ll)n * (n - 1) / 2);
	vector<pii> ed;
	if (m > (ll)n * n / 3) {
		for (int i = 0; i < (n); ++i) for (int j = 0; j < (i); ++j) {
			int a = i, b = j;
			if (rand_bool()) swap(a, b);
			ed.push_back({a,b});
		}
		shuffle_vec(ed);
		ed.erase(ed.begin() + m, ed.end());
	} else {
		set<pii> seen;
		for (int i = 0; i < (m); ++i) {
			int a = rand_range(n);
			int b = rand_range(n);
			if (a == b) continue;
			if (!seen.insert(minmax(a, b)).second) continue;
			ed.push_back({a,b});
		}
	}
	return ed;
}

vector<vector<int>> random_simple_graph(int n, int m) {
	vector<vector<int>> ed(n);
	for (auto pa : random_simple_graph_as_edge_list(n, m)) {
		ed[pa.first].push_back(pa.second);
		ed[pa.second].push_back(pa.first);
	}
	for (auto& v : ed) shuffle_vec(v);
	return ed;
}

vector<pii> random_regular_graph_as_edge_list(int n, int k) {
	// TODO: this is slow and requires a lot of retries for large n, change to
	// something smarter.
	assert(k < n);
	vector<pii> ed;
	vector<int> cands(n), rem(n, k);
	for (int i = 0; i < (n); ++i) cands[i] = i;
	int failures = 0;
	set<pii> seen;
	while (!cands.empty()) {
		if ((int)(cands).size() == 1) goto fail;
		int ai = rand_range((int)(cands).size());
		int bi = rand_range((int)(cands).size());
		int a = cands[ai], b = cands[bi];
		if (a == b) continue;
		if (!seen.insert(minmax(a, b)).second) {
			if (failures++ > 100) goto fail;
			continue;
		}
		failures = 0;
		ed.push_back({a, b});
		--rem[a], --rem[b];
		if (ai < bi) swap(ai, bi), swap(a, b);
		if (rem[a] == 0) {
			swap(cands[ai], cands.back());
			cands.pop_back();
		}
		if (rem[b] == 0) {
			swap(cands[bi], cands.back());
			cands.pop_back();
		}
	}
	assert((int)(ed).size() == n * k / 2);
	return ed;
fail:
	cerr << "retry" << endl;
	return random_regular_graph_as_edge_list(n, k);
}
