/**
 * Author: simon lindholm
 * Date: 2015-05-12
 * License: CC0
 * Source: own work
 * Description:
 * find the smallest i in $[a,b]$ that maximizes $f(i)$, assuming that $f(a) < \dots < f(i) \ge \dots \ge f(b)$.
 * to reverse which of the sides allows non-strict inequalities, change the < marked with (A) to <=, and reverse the loop at (B).
 * to minimize $f$, change it to >, also at (B).
 * Usage:
	int ind = tern_search(1,n,[\&](int i){return a[i];});
 * Time: O(\log(b-a))
 * Status: tested
 */
#pragma once

template<class F>
int tern_search(int a, int b, F f) {
	assert(a <= b);
	while (b - a >= 5) {
		int mid = (a + b) / 2;
		if (f(mid) < f(mid+1)) a = mid; // (A)
		else b = mid+1;
	}
	for (int i = a+1; i < (b+1); ++i) if (f(a) < f(i)) a = i; // (B)
	return a;
}
