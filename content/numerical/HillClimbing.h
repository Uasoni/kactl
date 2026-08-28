/**
 * Author: simon lindholm
 * Date: 2015-02-04
 * License: CC0
 * Source: johan sannemo
 * Description: poor man's optimization for unimodal functions.
 * Status: used with great success
 */
#pragma once

template<class F>
pair<double, array<double, 2>> hill_climb(array<double, 2> start, F f) {
	pair<double, array<double, 2>> cur(f(start), start);
	for (double jmp = 1e9; jmp > 1e-20; jmp /= 2) {
		for (int j = 0; j < (100); ++j) for (int dx = -1; dx < (2); ++dx) for (int dy = -1; dy < (2); ++dy) {
			array<double, 2> p = cur.second;
			p[0] += dx*jmp;
			p[1] += dy*jmp;
			cur = min(cur, make_pair(f(p), p));
		}
	}
	return cur;
}
