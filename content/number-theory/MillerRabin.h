/**
 * Author: chilli, c1729, simon lindholm
 * Date: 2019-03-28
 * License: CC0
 * Source: wikipedia, https://miller-rabin.appspot.com/
 * Description: deterministic miller-rabin primality test.
 * guaranteed to work for numbers up to $7 \cdot 10^{18}$; for larger numbers, use python and extend A randomly.
 * Time: 7 times the complexity of $a^b \mod c$.
 * Status: stress-tested
 */
#pragma once

#include "ModMulLL.h"

bool is_prime(ull n) {
	if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
	ull bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022},
	    s = __builtin_ctzll(n-1), d = n >> s;
	for (ull a : bases) {   // ^ count trailing zeroes
		ull p = mod_pow(a%n, d, n), i = s;
		while (p != 1 && p != n - 1 && a % n && i--)
			p = mod_mul(p, p, n);
		if (p != n-1 && i != s) return 0;
	}
	return 1;
}
