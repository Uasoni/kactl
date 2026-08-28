/**
 * Author: simon lindholm
 * Date: 2020-05-30
 * License: CC0
 * Source: https://en.wikipedia.org/wiki/barrett_reduction
 * Description: compute $a \% b$ about 5 times faster than usual, where $b$ is constant but not known at compile time.
 * returns a value congruent to $a \pmod b$ in the range $[0, 2b)$.
 * Status: proven correct, stress-tested
 * measured as having 4 times lower latency, and 8 times higher throughput, see stress-test.
 * Details:
 * more precisely, it can be proven that the result equals 0 only if $a = 0$,
 * and otherwise lies in $[1, (1 + a/2^64) * b)$.
 */
#pragma once

typedef unsigned long long ull;
struct FastMod {
	ull b, m;
	FastMod(ull b) : b(b), m(-1ULL / b) {}
	ull reduce(ull a) { // a % b + (0 or b)
		return a - (ull)((__uint128_t(m) * a) >> 64) * b;
	}
};
