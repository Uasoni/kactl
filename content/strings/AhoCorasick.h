/**
 * Author: simon lindholm
 * Date: 2015-02-18
 * License: CC0
 * Source: marian's (TC) code
 * Description: aho-corasick automaton, used for multiple pattern matching.
 * initialize with AhoCorasick ac(patterns). Pattern and text positions are
 * numbered from 1. find(word) returns a vector with index 0 unused; each entry
 * is the index of the longest pattern ending there, or 0 if none.
 * find\_all($-$, word) finds all words (up to $N \sqrt N$ many if no duplicate patterns)
 * that start at each position (shortest first).
 * duplicate patterns are allowed; empty patterns are not.
 * patterns is a normal vector (no dummy element); returned ID j means
 * patterns[j-1]. Pass the same patterns, in the same order, to find\_all.
 * Default alphabet is A--Z; change FIRST\_CHARACTER for lowercase input.
 * All pattern/text characters must lie in the configured alphabet.
 * to find the longest words that start at each position, reverse all input.
 * for large alphabets, split each symbol into chunks, with sentinel bits for symbol boundaries.
 * Time: construction takes $O(26N)$, where $N =$ sum of length of patterns.
 * find(x) is $O(N)$, where N = length of x. find\_all is $O(NM)$.
 * Status: stress-tested
 */
#pragma once

struct AhoCorasick {
	private:
	enum { ALPHABET_SIZE = 26, FIRST_CHARACTER = 'A' }; // change this!
	struct Node {
		// (nmatches is optional)
		int back, next[ALPHABET_SIZE], start = 0, end = 0, nmatches = 0;
		Node(int v) { memset(next, v, sizeof(next)); }
	};
	vector<Node> nodes;
	vector<int> backp = vector<int>(1);
	void insert(const string& s, int j) {
		assert(!s.empty());
		int n = 0;
		for (char c : s) {
			int& m = nodes[n].next[c - FIRST_CHARACTER];
			if (m == -1) { n = m = (int)nodes.size(); nodes.emplace_back(-1); }
			else n = m;
		}
		if (nodes[n].end == 0) nodes[n].start = j;
		backp.push_back(nodes[n].end);
		nodes[n].end = j;
		nodes[n].nmatches++;
	}
	public:
	AhoCorasick(const vector<string>& pat) : nodes(1, -1) {
		for (int i = 0; i < (int)pat.size(); ++i) insert(pat[i], i + 1);
		nodes[0].back = (int)nodes.size();
		nodes.emplace_back(0);

		queue<int> q;
		for (q.push(0); !q.empty(); q.pop()) {
			int n = q.front(), prev = nodes[n].back;
			for (int i = 0; i < ALPHABET_SIZE; ++i) {
				int &ed = nodes[n].next[i], y = nodes[prev].next[i];
				if (ed == -1) ed = y;
				else {
					nodes[ed].back = y;
					(nodes[ed].end == 0 ? nodes[ed].end : backp[nodes[ed].start])
						= nodes[y].end;
					nodes[ed].nmatches += nodes[y].nmatches;
					q.push(ed);
				}
			}
		}
	}
	vector<int> find(string word) {
		int n = 0;
		vector<int> res(1); // ll count = 0;
		for (char c : word) {
			n = nodes[n].next[c - FIRST_CHARACTER];
			res.push_back(nodes[n].end);
			// count += nodes[n].nmatches; // Includes overlaps.
		}
		return res;
	}
	vector<vector<int>> find_all(vector<string>& pat, string word) {
		vector<int> r = find(word);
		vector<vector<int>> res(word.size() + 1);
		for (int i = 1; i <= (int)word.size(); ++i) {
			int ind = r[i];
			while (ind) {
				res[i - (int)pat[ind-1].size() + 1].push_back(ind);
				ind = backp[ind];
			}
		}
		return res;
	}
};
