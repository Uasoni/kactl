/**
 * Author: lucian bicsi
 * Date: 2017-10-31
 * License: CC0
 * Source: wikipedia
 * Description: recovers any $n$-order linear recurrence relation from the first
 * $2n$ terms of the recurrence.
 * useful for guessing linear recurrences after brute-forcing the first terms.
 * should work on any field, but numerical stability for floats is not guaranteed.
 * output will have size $\le n$.
 * Usage: berlekamp_massey({0, 1, 1, 3, 5, 11}, MOD) // {1, 2}
 * Time: O(N^2)
 * Status: BruteForce-tested mod 5 for n <= 5 and all s
 */
#pragma once

vector<ll> berlekamp_massey(vector<ll> sequence, ll modulus) {
	int n = (int)sequence.size(), length = 0, shift = 0;
	vector<ll> current(n), previous(n), backup;
	current[0] = previous[0] = 1;
	auto mod_pow = [&](ll base, ll exponent) {
		ll result = 1;
		for (; exponent; exponent /= 2, base = base * base % modulus)
			if (exponent & 1) result = result * base % modulus;
		return result;
	};

	ll b = 1;
	for (int i = 0; i < n; ++i) { ++shift;
		ll d = sequence[i] % modulus;
		for (int j = 1; j < length + 1; ++j)
			d = (d + current[j] * sequence[i - j]) % modulus;
		if (!d) continue;
		backup = current;
		ll coefficient = d * mod_pow(b, modulus - 2) % modulus;
		for (int j = shift; j < n; ++j)
			current[j] = (current[j]
				- coefficient * previous[j - shift]) % modulus;
		if (2 * length > i) continue;
		length = i + 1 - length;
		previous = backup; b = d; shift = 0;
	}

	current.resize(length + 1); current.erase(current.begin());
	for (ll& x : current) x = (modulus - x) % modulus;
	return current;
}
