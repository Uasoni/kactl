/**
 * Author: chilli, SJTU, janez konc
 * Date: 2019-05-10
 * License: GPL3+
 * Source: https://en.wikipedia.org/wiki/max_clique_dyn_maximum_clique_algorithm, https://gitlab.com/janezkonc/mcqd/blob/master/mcqd.h
 * Description: quickly finds a maximum clique of a graph (given as symmetric bitset
 * matrix; self-edges not allowed). can be used to find a maximum independent
 * set by finding a clique of the complement graph.
 * vertices are numbered $1..n$ and the matrix has size $n+1$.
 * Time: runs in about 1s for n=155 and worst case random graphs (p=.90). runs
 * faster for sparse graphs.
 * Status: stress-tested
 */
struct MaximumClique {
	double limit=0.025, pk=0;
	struct Vertex { int i, d=0; };
	vector<bitset<200>> e;
	vector<Vertex> vertices;
	vector<vector<int>> colors;
	vector<int> maximum, current, steps, old_steps;
	void init(vector<Vertex>& r) {
		for (auto& v : r) v.d = 0;
		for (auto& v : r) for (auto j : r) v.d += e[v.i][j.i];
		sort(begin(r), end(r), [](auto a, auto b) { return a.d > b.d; });
		int mx_d = r[0].d;
		for (int i = 0; i < ((int)(r).size()); ++i) r[i].d = min(i, mx_d) + 1;
	}
	void expand(vector<Vertex>& remaining, int level = 1) {
		steps[level] += steps[level - 1] - old_steps[level];
		old_steps[level] = steps[level - 1];
		while (!remaining.empty()) {
			if ((int)current.size() + remaining.back().d <= (int)maximum.size()) return;
			current.push_back(remaining.back().i);
			vector<Vertex> next_vertices;
			for(auto v:remaining) if (e[remaining.back().i][v.i]) next_vertices.push_back({v.i});
			if (!next_vertices.empty()) {
				if (steps[level]++ / ++pk < limit) init(next_vertices);
				int j = 0, mxk = 1, mnk = max((int)maximum.size() - (int)current.size() + 1, 1);
				colors[1].clear(), colors[2].clear();
				for (auto v : next_vertices) {
					int k = 1;
					auto f = [&](int i) { return e[v.i][i]; };
					while (any_of(begin(colors[k]), end(colors[k]), f)) k++;
					if (k > mxk) mxk = k, colors[mxk + 1].clear();
					if (k < mnk) next_vertices[j++].i = v.i;
					colors[k].push_back(v.i);
				}
				if (j > 0) next_vertices[j - 1].d = 0;
				for (int k = mnk; k <= mxk; ++k) for (int i : colors[k])
					next_vertices[j].i = i, next_vertices[j++].d = k;
				expand(next_vertices, level + 1);
			} else if (current.size() > maximum.size()) maximum = current;
			current.pop_back(), remaining.pop_back();
		}
	}
	vector<int> max_clique() { init(vertices), expand(vertices); return maximum; }
	MaximumClique(vector<bitset<200>> conn) : e(conn), colors(e.size() + 1),
		steps(colors.size()), old_steps(steps) {
		for (int i = 1; i < (int)e.size(); ++i) vertices.push_back({i});
	}
};
