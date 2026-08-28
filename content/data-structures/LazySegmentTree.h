/**
 * Author: simon lindholm
 * Date: 2016-10-08
 * License: CC0
 * Source: me
 * Description: segment tree with ability to add or set values of large intervals, and compute max of intervals.
 * can be changed to other things.
 * use with a bump allocator for better performance, and small\_ptr or implicit indices to save memory.
 * Time: O(\log N).
 * Usage: LazySegmentTree tree(values); // public ranges are 1-indexed inclusive
 * Status: stress-tested a bit
 */
#pragma once

#include "../various/BumpAllocator.h"

const int INF = 1e9;
struct LazySegmentTree {
	LazySegmentTree *left_child = 0, *right_child = 0;
	int low, high, pending_set = INF, pending_add = 0, value = -INF;
	LazySegmentTree(int low, int high) : low(low), high(high) {}
	LazySegmentTree(const vector<int>& values) :
		LazySegmentTree(values, 1, (int)values.size() - 1) {}
	LazySegmentTree(const vector<int>& values, int low, int high) :
		low(low), high(high) {
		if (low < high) {
			int mid = low + (high - low) / 2;
			left_child = new LazySegmentTree(values, low, mid);
			right_child = new LazySegmentTree(values, mid + 1, high);
			value = max(left_child->value, right_child->value);
		}
		else value = values[low];
	}
	int query(int left, int right) {
		if (right < low || high < left) return -INF;
		if (left <= low && high <= right) return value;
		push();
		return max(left_child->query(left, right),
			right_child->query(left, right));
	}
	void set(int left, int right, int x) {
		if (right < low || high < left) return;
		if (left <= low && high <= right)
			pending_set = value = x, pending_add = 0;
		else {
			push();
			left_child->set(left, right, x);
			right_child->set(left, right, x);
			value = max(left_child->value, right_child->value);
		}
	}
	void add(int left, int right, int x) {
		if (right < low || high < left) return;
		if (left <= low && high <= right) {
			if (pending_set != INF) pending_set += x;
			else pending_add += x;
			value += x;
		}
		else {
			push();
			left_child->add(left, right, x);
			right_child->add(left, right, x);
			value = max(left_child->value, right_child->value);
		}
	}
	void push() {
		if (!left_child) {
			int mid = low + (high - low) / 2;
			left_child = new LazySegmentTree(low, mid);
			right_child = new LazySegmentTree(mid + 1, high);
		}
		if (pending_set != INF) {
			left_child->set(low, high, pending_set);
			right_child->set(low, high, pending_set);
			pending_set = INF;
		} else if (pending_add) {
			left_child->add(low, high, pending_add);
			right_child->add(low, high, pending_add);
			pending_add = 0;
		}
	}
};
