/**
 * Author: lucian bicsi
 * Date: 2017-10-31
 * License: CC0
 * Source: folklore
 * Description: 1-indexed max-tree with inclusive range bounds.
 * can be changed by modifying value\_type, f and UNIT.
 * Time: O(\log N)
 * Status: stress-tested
 */
#pragma once

struct SegmentTree {
	typedef int value_type;
	static constexpr value_type UNIT = INT_MIN;
	value_type f(value_type a, value_type b) { return max(a, b); }
	vector<value_type> s; int n;
	SegmentTree(int n = 0, value_type def = UNIT) : s(2*n, def), n(n) {}
	void update(int pos, value_type val) {
		for (s[pos += n - 1] = val; pos /= 2;)
			s[pos] = f(s[pos * 2], s[pos * 2 + 1]);
	}
	value_type query(int left, int right) { // query [left, right]
		value_type left_result = UNIT, right_result = UNIT;
		for (left += n - 1, right += n; left < right;
				left /= 2, right /= 2) {
			if (left % 2) left_result = f(left_result, s[left++]);
			if (right % 2) right_result = f(s[--right], right_result);
		}
		return f(left_result, right_result);
	}
};
