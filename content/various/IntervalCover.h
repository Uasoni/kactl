/**
 * Author: johan sannemo
 * License: CC0
 * Description: compute indices of smallest set of intervals covering another interval.
 * intervals and the goal are [inclusive, inclusive] integer ranges.
 * returns 1-indexed interval positions, or an empty set on failure (or if the
 * goal is empty).
 * Time: O(N \log N)
 * Status: tested on kattis:intervalcover
 */
#pragma once

vector<int> cover(pii goal, vector<pii> intervals) {
	vector<int> order((int)intervals.size()), result;
	iota(begin(order), end(order), 0);
	sort(begin(order), end(order), [&](int a, int b) {
		return intervals[a] < intervals[b];
	});
	int current = goal.first;
	int at = 0;
	while (current <= goal.second) {
		pii best = {current - 1, -1};
		while (at < (int)intervals.size()
				&& intervals[order[at]].first <= current) {
			best = max(best, {intervals[order[at]].second, order[at]});
			at++;
		}
		if (best.second == -1) return {};
		current = best.first + 1;
		result.push_back(best.second + 1);
	}
	return result;
}
