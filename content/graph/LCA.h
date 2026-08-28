/**
 * Author: chilli, pajenegod
 * Date: 2020-02-20
 * License: CC0
 * Source: folklore
 * Description: data structure for computing lowest common ancestors in a tree
 * (with 1 as root). graph should have size $n+1$ and use vertices $1..n$.
 * or undirected.
 * Time: $O(N \log N + Q)$
 * Status: stress-tested
 */
#pragma once

#include "../data-structures/RMQ.h"

struct LcaQuery {

private:
	int timer = 0;
	vector<int> time, path = vector<int>(1), ret = vector<int>(1);
	RangeMinimumQuery<int> range_minimum;

	void dfs(vector<vector<int>>& graph, int v, int par) {
		time[v] = ++timer;
		for (int y : graph[v]) if (y != par) {
			path.push_back(v), ret.push_back(time[v]);
			dfs(graph, y, v);
		}
	}


public:
	LcaQuery(vector<vector<int>>& graph) : time((int)graph.size()),
		range_minimum((dfs(graph, 1, 0), ret)) {}
	int size() const { return (int)time.size() - 1; }
	int entry_time(int vertex) const { return time[vertex]; }
	int query(int a, int b) {
		if (a == b) return a;
		tie(a, b) = minmax(time[a], time[b]);
		return path[range_minimum.query(a, b - 1)];
	}
	// dist(a,b){return depth[a] + depth[b] - 2*depth[query(a,b)];}
};
