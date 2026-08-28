/**
 * Author: simon lindholm
 * License: CC0
 * Description: add and remove intervals from a set of disjoint intervals.
 * will merge the added interval with any overlapping intervals in the set when adding.
 * intervals are [inclusive, inclusive].
 * Time: O(\log N)
 * Status: stress-tested
 */
#pragma once

set<pii>::iterator add_interval(set<pii>& intervals, int left, int right) {
	if (left > right) return intervals.end();
	auto it = intervals.lower_bound({left, right}), before = it;
	while (it != intervals.end() && it->first <= right + 1) {
		right = max(right, it->second);
		before = it = intervals.erase(it);
	}
	if (it != intervals.begin() && (--it)->second + 1 >= left) {
		left = min(left, it->first);
		right = max(right, it->second);
		intervals.erase(it);
	}
	return intervals.insert(before, {left, right});
}

void remove_interval(set<pii>& intervals, int left, int right) {
	if (left > right) return;
	auto it = add_interval(intervals, left, right);
	int old_left = it->first, old_right = it->second;
	intervals.erase(it);
	if (old_left < left) intervals.emplace(old_left, left - 1);
	if (right < old_right) intervals.emplace(right + 1, old_right);
}
