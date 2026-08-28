/**
 * Author: lucian bicsi
 * Date: 2018-02-14
 * License: CC0
 * Source: chinese material
 * Description: generates the $k$'th term of an $n$-order
 * linear recurrence $S[i] = \sum_j S[i-j-1]tr[j]$,
 * given $S[0 \ldots \ge n-1]$ and $tr[0 \ldots n-1]$.
 * faster than matrix multiplication.
 * useful together with berlekamp--massey.
 * Usage: linear_rec({0, 1}, {1, 1}, k) // k'th fibonacci number
 * Time: O(n^2 \log k)
 * Status: BruteForce-tested mod 5 for n <= 5
 */
#pragma once

const ll MOD = 5; /** exclude-line */
ll linear_rec(vector<ll> sequence, vector<ll> transition, ll k) {
	int n = (int)transition.size();

	auto combine = [&](vector<ll> a, vector<ll> b) {
		vector<ll> res(n * 2 + 1);
		for (int i = 0; i < (n+1); ++i) for (int j = 0; j < (n+1); ++j)
			res[i + j] = (res[i + j] + a[i] * b[j]) % MOD;
		for (int i = 2 * n; i > n; --i) for (int j = 0; j < (n); ++j)
			res[i - 1 - j] = (res[i - 1 - j] + res[i] * transition[j]) % MOD;
		res.resize(n + 1);
		return res;
	};

	vector<ll> pol(n + 1), e(pol);
	pol[0] = e[1] = 1;

	for (++k; k; k /= 2) {
		if (k % 2) pol = combine(pol, e);
		e = combine(e, e);
	}

	ll res = 0;
	for (int i = 0; i < (n); ++i)
		res = (res + pol[i + 1] * sequence[i]) % MOD;
	return res;
}
