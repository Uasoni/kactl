/**
 * Author: chilli, ramchandra apte, noam527, simon lindholm
 * Date: 2019-04-24
 * License: CC0
 * Source: https://github.com/ramchandra_apte/omni_template/blob/master/src/number_theory/modulo.hpp
 * Description: calculate $a\cdot b\bmod c$ (or $a^b \bmod c$) for $0 \le a, b \le c \le 7.2\cdot 10^{18}$.
 * Time: O(1) for \texttt{modmul}, O(\log b) for \texttt{modpow}
 * Status: stress-tested, proven correct
 * Details:
 * this runs ~2x faster than the naive (\_\_int128\_t)a * b \% M.
 * A proof of correctness is in doc/modmul-proof.tex. an earlier version of the proof,
 * from when the code used a * b / (long double)M, is in doc/modmul-proof.md.
 * the proof assumes that long doubles are implemented as x87 80-bit floats; if they
 * are 64-bit, as on e.g. MSVC, the implementation is only valid for
 * $0 \le a, b \le c < 2^{52} \approx 4.5 \cdot 10^{15}$.
 */
#pragma once

typedef unsigned long long ull;
ull mod_mul(ull a, ull b, ull modulus) {
	ll ret = a * b - modulus * ull(1.L / modulus * a * b);
	return ret + modulus * (ret < 0) - modulus * (ret >= (ll)modulus);
}
ull mod_pow(ull b, ull e, ull mod) {
	ull ans = 1;
	for (; e; b = mod_mul(b, b, mod), e /= 2)
		if (e & 1) ans = mod_mul(ans, b, mod);
	return ans;
}
