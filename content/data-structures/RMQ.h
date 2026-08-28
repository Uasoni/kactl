/**
 * Author: johan sannemo, pajenegod
 * Date: 2015-02-06
 * License: CC0
 * Source: folklore
 * Description: range minimum queries on an array. returns
 * min(V[a], V[a+1], ... V[b]) in constant time. Input and query positions are
 * 1-indexed and V[0] is ignored.
 * Usage:
 *  RangeMinimumQuery RangeMinimumQuery(values);
 *  RangeMinimumQuery.query(inclusive, inclusive); // 1-indexed
 * Time: $O(|V| \log |V| + Q)$
 * Status: stress-tested
 */
#pragma once

template<class T>
struct RangeMinimumQuery {

private:
	vector<vector<T>> jmp;

public:
	RangeMinimumQuery(const vector<T>& values) :
		jmp(1, vector<T>(next(begin(values)), end(values))) {
		for (int pw = 1, k = 1; pw * 2 <= (int)jmp[0].size(); pw *= 2, ++k) {
			jmp.emplace_back((int)jmp[0].size() - pw * 2 + 1);
			for (int j = 0; j < ((int)(jmp[k]).size()); ++j)
				jmp[k][j] = min(jmp[k - 1][j], jmp[k - 1][j + pw]);
		}
	}
	T query(int left, int right) {
		assert(1 <= left && left <= right && right <= (int)jmp[0].size());
		--left;
		int depth = 31 - __builtin_clz(right - left);
		return min(jmp[depth][left], jmp[depth][right - (1 << depth)]);
	}
};
