/**
 * Author: unknown
 * Date: 2002-09-13
 * Source: predates tiny_kactl
 * Description: topological sorting. given is an oriented graph.
 * output is an ordering of vertices, such that there are edges only from left to right.
 * if there are cycles, the returned list will have size smaller than $n$ -- nodes reachable
 * from cycles will not be returned. Vertices are numbered $1..n$ and the
 * adjacency list has size $n+1$.
 * Time: $O(|V|+|E|)$
 * Status: stress-tested
 */
#pragma once

vector<int> topo_sort(const vector<vector<int>>& gr) {
	vector<int> indeg((int)(gr).size()), q;
	for (int i = 1; i < (int)gr.size(); ++i) for (int x : gr[i]) indeg[x]++;
	for (int i = 1; i < (int)gr.size(); ++i) if (indeg[i] == 0) q.push_back(i);
	for (int j = 0; j < ((int)(q).size()); ++j) for (int x : gr[q[j]])
		if (--indeg[x] == 0) q.push_back(x);
	return q;
}
