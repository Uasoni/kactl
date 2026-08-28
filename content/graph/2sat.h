/**
 * Author: emil lenngren, simon lindholm
 * Date: 2011-11-29
 * License: CC0
 * Source: folklore
 * Description: calculates a valid assignment to boolean variables a, b, c,... to a 2-SAT problem,
 * so that an expression of the type $(a||b)\&\&(!a||c)\&\&(d||!b)\&\&...$
 * becomes true, or reports that it is unsatisfiable.
 * variables are numbered $1..n$ and negation is represented by a minus sign.
 * Usage:
 *  TwoSat ts(number of boolean variables);
 *  ts.either(1, -4); // var 1 is true or var 4 is false
 *  ts.set_value(3); // var 3 is true
 *  ts.at_most_one({1,-2,3}); // <= 1 of these literals is true
 *  ts.solve(); // returns true iff it is solvable
 *  ts.values[1..n] holds the assigned values to the vars
 * Time: O(N+E), where N is the number of boolean variables, and E is the number of clauses.
 * Status: stress-tested
 */
#pragma once

struct TwoSat {
	int variable_count;
	vector<vector<int>> gr;
	vector<int> values; // 0 = false, 1 = true

	TwoSat(int n = 0) : variable_count(n), gr(2*n) {}

	int add_var() { // (optional)
		gr.emplace_back();
		gr.emplace_back();
		return ++variable_count;
	}

	int literal_node(int literal) {
		assert(literal != 0);
		return 2 * (abs(literal) - 1) + (literal < 0);
	}
	void either(int f, int j) {
		f = literal_node(f);
		j = literal_node(j);
		gr[f].push_back(j^1);
		gr[j].push_back(f^1);
	}
	void set_value(int x) { either(x, x); }

	void at_most_one(const vector<int>& li) { // (optional)
		if ((int)(li).size() <= 1) return;
		int cur = -li[0];
		for (int i = 2; i < ((int)(li).size()); ++i) {
			int next = add_var();
			either(cur, -li[i]);
			either(cur, next);
			either(-li[i], next);
			cur = -next;
		}
		either(cur, -li[1]);
	}

	vector<int> val, comp, z; int time = 0;
	int dfs(int i) {
		int low = val[i] = ++time, x; z.push_back(i);
		for(int e : gr[i]) if (!comp[e])
			low = min(low, val[e] ?: dfs(e));
		if (low == val[i]) do {
			x = z.back(); z.pop_back();
			comp[x] = low;
			if (values[(x >> 1) + 1] == -1)
				values[(x >> 1) + 1] = x & 1;
		} while (x != i);
		return val[i] = low;
	}

	bool solve() {
		values.assign(variable_count + 1, -1);
		val.assign(2*variable_count, 0); comp = val;
		for (int i = 0; i < 2*variable_count; ++i) if (!comp[i]) dfs(i);
		for (int i = 0; i < variable_count; ++i)
			if (comp[2*i] == comp[2*i+1]) return 0;
		return 1;
	}
};
