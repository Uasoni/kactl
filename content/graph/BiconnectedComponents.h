/**
 * Author: simon lindholm
 * Date: 2017-04-17
 * License: CC0
 * Source: folklore
 * Description: finds all biconnected components in an undirected graph, and
 *  runs a callback for the edges in each. in a biconnected component there
 *  are at least two internally disjoint paths between any two nodes (a cycle
 *  exists through them). note that a node can be in several components. an
 *  edge which is not in a component is a bridge, i.e., not part of any cycle.
 * Usage:
 *  int edge_id = 1; ed.resize(n + 1);
 *  for each edge (a,b) {
 *    ed[a].emplace_back(b, edge_id);
 *    ed[b].emplace_back(a, edge_id++); }
 *  biconnected_components([\&](const vector<int>\& edges) {...});
 * vertices and edge IDs are numbered from 1.
 * Time: O(E + V)
 * Status: tested during MIPT ICPC workshop 2017
 */
#pragma once

vector<int> num, st;
vector<vector<pii>> ed;
int dfs_time;
template<class F>
int biconnected_dfs(int at, int parent_edge, F& f) {
	int me = num[at] = ++dfs_time, top = me;
	for (auto [y, edge_id] : ed[at]) if (edge_id != parent_edge) {
		if (num[y]) {
			top = min(top, num[y]);
			if (num[y] < me)
				st.push_back(edge_id);
		} else {
			int stack_size = (int)st.size();
			int up = biconnected_dfs(y, edge_id, f);
			top = min(top, up);
			if (up == me) {
				st.push_back(edge_id);
				f(vector<int>(st.begin() + stack_size, st.end()));
				st.resize(stack_size);
			}
			else if (up < me) st.push_back(edge_id);
			else { /* edge_id is a bridge */ }
		}
	}
	return top;
}

template<class F>
void biconnected_components(F f) {
	dfs_time = 0;
	num.assign((int)ed.size(), 0);
	for (int i = 1; i < (int)ed.size(); ++i)
		if (!num[i]) biconnected_dfs(i, 0, f);
}
