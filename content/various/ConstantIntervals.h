/**
 * Author: simon lindholm
 * Date: 2015-03-20
 * License: CC0
 * Source: me
 * Description: split a monotone function on [from, to] into a minimal set of
 *  inclusive intervals on which it has the same value.
 *  runs a callback g for each such interval.
 * Usage: constant_intervals(1, n, f, [\&](int lo, int hi, T val){...});
 * Time: O(k\log\frac{n}{k})
 * Status: tested
 */
#pragma once

template<class F, class G, class T>
void constant_intervals_rec(int from, int to, F& f, G& g,
		int& start, T& previous, T current) {
	if (previous == current) return;
	if (from == to) {
		g(start, to - 1, previous);
		start = to; previous = current;
	} else {
		int mid = (from + to) >> 1;
		constant_intervals_rec(from, mid, f, g, start, previous, f(mid));
		constant_intervals_rec(mid + 1, to, f, g, start, previous, current);
	}
}
template<class F, class G>
void constant_intervals(int from, int to, F f, G g) {
	if (to < from) return;
	int start = from;
	auto previous = f(from), current = f(to);
	constant_intervals_rec(from, to, f, g, start, previous, current);
	g(start, to, current);
}
