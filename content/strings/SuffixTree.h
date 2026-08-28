/**
 * Author: unknown
 * Date: 2017-05-15
 * Source: https://e-maxx.ru/algo/ukkonen
 * Description: ukkonen's algorithm for online suffix tree construction.
 *  Public node IDs and string positions are 1-indexed. edge\_range(node)
 *  returns an inclusive-inclusive substring range; child returns 0 if absent.
 *  The root has ID 1 and no edge range.
 *  to get a complete tree, append a dummy symbol -- otherwise it may contain
 *  an incomplete path (still useful for substring matching, though).
 * Time: $O(28N)$
 * Status: stress-tested a bit
 */
#pragma once

struct SuffixTree {
	private:
	enum { MAX_N = 200010, ALPHABET_SIZE = 28 }; // MAX_N ~ 2*maxlen+10
	string text;
	int next_node[MAX_N][ALPHABET_SIZE], left[MAX_N], right[MAX_N];
	int parent[MAX_N], suffix_link[MAX_N];
	int active_node = 0, active_position = 0, node_total = 2;
	pii best;

	int to_index(char c) { return c - 'a'; }
	void add(int position, int character) { suffix:
		if (right[active_node] <= active_position) {
			if (next_node[active_node][character] == -1) {
				next_node[active_node][character] = node_total;
				left[node_total] = position; parent[node_total++] = active_node;
				active_node = suffix_link[active_node];
				active_position = right[active_node]; goto suffix;
			}
			active_node = next_node[active_node][character];
			active_position = left[active_node];
		}
		if (active_position == -1 || character == to_index(text[active_position]))
			active_position++;
		else {
			left[node_total+1] = position; parent[node_total+1] = node_total;
			left[node_total] = left[active_node]; right[node_total] = active_position;
			parent[node_total] = parent[active_node];
			next_node[node_total][character] = node_total + 1;
			next_node[node_total][to_index(text[active_position])] = active_node;
			left[active_node] = active_position; parent[active_node] = node_total;
			next_node[parent[node_total]][to_index(text[left[node_total]])] = node_total;
			active_node = suffix_link[parent[node_total]];
			active_position = left[node_total];
			while (active_position < right[node_total]) {
				active_node = next_node[active_node][to_index(text[active_position])];
				active_position += right[active_node] - left[active_node];
			}
			if (active_position == right[node_total]) suffix_link[node_total] = active_node;
			else suffix_link[node_total] = node_total + 2;
			active_position = right[active_node] - (active_position - right[node_total]);
			node_total += 2; goto suffix;
		}
	}
	int lcs_dfs(int node, int first_separator, int second_separator, int old_length) {
		if (left[node] <= first_separator && first_separator < right[node]) return 1;
		if (left[node] <= second_separator && second_separator < right[node]) return 2;
		int mask = 0;
		int length = node ? old_length + right[node] - left[node] : 0;
		for (int c = 0; c < ALPHABET_SIZE; ++c) if (next_node[node][c] != -1)
			mask |= lcs_dfs(next_node[node][c], first_separator, second_separator, length);
		if (mask == 3)
			best = max(best, {length, right[node] - length + 1});
		return mask;
	}

	public:
	SuffixTree(string text) : text(text) {
		fill(right, right+MAX_N, (int)text.size());
		memset(suffix_link, 0, sizeof suffix_link);
		memset(next_node, -1, sizeof next_node);
		fill(next_node[1], next_node[1]+ALPHABET_SIZE, 0);
		suffix_link[0] = 1;
		left[0] = left[1] = -1;
		right[0] = right[1] = parent[0] = parent[1] = 0;
		for (int i = 0; i < (int)text.size(); ++i) add(i, to_index(text[i]));
	}
	int size() const { return node_total - 1; }
	int child(int node, char c) const {
		int internal_node = node == 1 ? 0 : node;
		int result = next_node[internal_node][c - 'a'];
		return result == -1 ? 0 : result;
	}
	int parent_of(int node) const {
		int result = parent[node];
		return result == 0 ? 1 : result;
	}
	pii edge_range(int node) const {
		assert(node > 1);
		return {left[node] + 1, right[node]};
	}

	// find longest common substring; returns (length, 1-indexed start in s)
	static pii lcs(string s, string t) {
		auto tree = make_unique<SuffixTree>(s + (char)('z' + 1) + t + (char)('z' + 2));
		tree->lcs_dfs(0, (int)s.size(), (int)s.size() + 1 + (int)t.size(), 0);
		return tree->best;
	}
};
