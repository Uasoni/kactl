/**
 * Author: simon lindholm
 * Date: 2018-07-18
 * License: CC0
 * Source: https://en.wikipedia.org/wiki/bron%E2%80%93Kerbosch_algorithm
 * Description: runs a callback for all maximal cliques in a graph (given as a
 * symmetric bitset matrix; self-edges not allowed). callback is given a bitset
 * representing the maximal clique.
 * vertices are numbered $1..n$ and the matrix has size $n+1$.
 * Time: O(3^{n/3}), much faster for sparse graphs
 * Status: stress-tested
 */
#pragma once
/// possible optimization: on the top-most
/// recursion level, ignore 'cands', and go through nodes in order of increasing
/// degree, where degrees go down as nodes are removed.
/// (mostly irrelevant given MaximumClique)
template<class F>
void cliques_rec(vector<bitset<128>>& eds, F& f, bitset<128> possible,
		bitset<128> excluded, bitset<128> current) {
	if (!possible.any()) { if (!excluded.any()) f(current); return; }
	auto q = (possible | excluded)._Find_first();
	auto candidates = possible & ~eds[q];
	for (int i = 1; i < (int)eds.size(); ++i) if (candidates[i]) {
		current[i] = 1;
		cliques_rec(eds, f, possible & eds[i], excluded & eds[i], current);
		current[i] = possible[i] = 0; excluded[i] = 1;
	}
}

template<class F>
void cliques(vector<bitset<128>>& eds, F f) {
	bitset<128> possible;
	for (int i = 1; i < (int)eds.size(); ++i) possible[i] = 1;
	cliques_rec(eds, f, possible, {}, {});
}
