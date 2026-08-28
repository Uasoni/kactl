/**
 * Author: chilli
 * Date: 2019-04-16
 * License: CC0
 * Source: based on KACTL's FFT
 * Description: ntt(a) computes $\hat f(k) = \sum_x a[x] g^{xk}$ for all $k$, where $g=\text{root}^{(mod-1)/N}$.
 * N must be a power of 2.
 * useful for convolution modulo specific nice primes of the form $2^a b+1$,
 * where the convolution result has size at most $2^a$. for arbitrary modulo, see fft\_mod.
   \texttt{conv(a, b) = c}, where $c[x] = \sum a[i]b[x-i]$.
   for manual convolution: NTT the inputs, multiply
   pointwise, divide by n, reverse(start+1, end), NTT back.
 * inputs must be in [0, mod).
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

const ll MOD = (119 << 23) + 1, ROOT = 62; // = 998244353
// for p < 2^30 there is also e.g. 5 << 25, 7 << 26, 479 << 21
// and 483 << 21 (same root). the last two are > 10^9.
ll ntt_mod_pow(ll base, ll exponent) {
	ll result = 1;
	for (; exponent; exponent /= 2, base = base * base % MOD)
		if (exponent & 1) result = result * base % MOD;
	return result;
}
void ntt(vector<ll> &a) {
	int n = (int)(a).size(), log_size = 31 - __builtin_clz(n);
	static vector<ll> rt(2, 1);
	for (static int k = 2, s = 2; k < n; k *= 2, s++) {
		rt.resize(n);
		ll z[] = {1, ntt_mod_pow(ROOT, MOD >> s)};
		for (int i = k; i < (2*k); ++i) rt[i] = rt[i / 2] * z[i & 1] % MOD;
	}
	vector<int> rev(n);
	for (int i = 0; i < n; ++i) rev[i] = (rev[i / 2] | (i & 1) << log_size) / 2;
	for (int i = 0; i < (n); ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k *= 2)
		for (int i = 0; i < n; i += 2 * k) for (int j = 0; j < (k); ++j) {
			ll z = rt[j + k] * a[i + j + k] % MOD, &ai = a[i + j];
			a[i + j + k] = ai - z + (z > ai ? MOD : 0);
			ai += (ai + z >= MOD ? z - MOD : z);
		}
}
vector<ll> conv(const vector<ll> &a, const vector<ll> &b) {
	if (a.empty() || b.empty()) return {};
	int s = (int)(a).size() + (int)(b).size() - 1;
	int log_size = 32 - __builtin_clz(s), n = 1 << log_size;
	int inv = ntt_mod_pow(n, MOD - 2);
	vector<ll> left(a), right(b), out(n);
	left.resize(n), right.resize(n);
	ntt(left), ntt(right);
	for (int i = 0; i < (n); ++i)
		out[-i & (n - 1)] = (ll)left[i] * right[i] % MOD * inv % MOD;
	ntt(out);
	return {out.begin(), out.begin() + s};
}
