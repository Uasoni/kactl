/**
 * Author: ludo pulles, chilli, simon lindholm
 * Date: 2019-01-09
 * License: CC0
 * Source: http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf (do read, it's excellent)
   accuracy bound from http://www.daemonology.net/papers/fft.pdf
 * Description: fft(a) computes $\hat f(k) = \sum_x a[x] \exp(2\pi i \cdot k x / N)$ for all $k$. N must be a power of 2.
   useful for convolution:
   \texttt{conv(a, b) = c}, where $c[x] = \sum a[i]b[x-i]$.
   for convolution of complex numbers or more than two vectors: FFT, multiply
   pointwise, divide by n, reverse(start+1, end), FFT back.
   rounding is safe if $(\sum a_i^2 + \sum b_i^2)\log_2{N} < 9\cdot10^{14}$
   (in practice $10^{16}$; higher for random inputs).
   otherwise, use NTT/fft\_mod.
 * Time: O(N \log N) with $N = |A|+|B|$ ($\tilde 1s$ for $N=2^{22}$)
 * Status: somewhat tested
 * Details: an in-depth examination of precision for both FFT and fft_mod can be found
 * here (https://github.com/simonlindholm/fft-precision/blob/master/fft-precision.md)
 */
#pragma once

typedef complex<double> C;
void fft(vector<C>& a) {
	int n = (int)(a).size(), log_size = 31 - __builtin_clz(n);
	static vector<complex<long double>> roots(2, 1);
	static vector<C> rt(2, 1);  // (^ 10% faster if double)
	for (static int k = 2; k < n; k *= 2) {
		roots.resize(n); rt.resize(n);
		auto x = polar(1.0L, acos(-1.0L) / k);
		for (int i = k; i < (2*k); ++i)
			rt[i] = roots[i] = i&1 ? roots[i/2] * x : roots[i/2];
	}
	vector<int> rev(n);
	for (int i = 0; i < n; ++i) rev[i] = (rev[i / 2] | (i & 1) << log_size) / 2;
	for (int i = 0; i < (n); ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k *= 2)
		for (int i = 0; i < n; i += 2 * k) for (int j = 0; j < (k); ++j) {
			// C z = rt[j+k] * a[i+j+k]; // (25% faster if hand-rolled)  /// include-line
			auto x = (double *)&rt[j+k], y = (double *)&a[i+j+k];        /// exclude-line
			C z(x[0]*y[0] - x[1]*y[1], x[0]*y[1] + x[1]*y[0]);           /// exclude-line
			a[i + j + k] = a[i + j] - z;
			a[i + j] += z;
		}
}
vector<double> conv(const vector<double>& a, const vector<double>& b) {
	if (a.empty() || b.empty()) return {};
	vector<double> res((int)(a).size() + (int)(b).size() - 1);
	int log_size = 32 - __builtin_clz((int)res.size()), n = 1 << log_size;
	vector<C> in(n), out(n);
	copy(begin(a), end(a), begin(in));
	for (int i = 0; i < ((int)(b).size()); ++i) in[i].imag(b[i]);
	fft(in);
	for (C& x : in) x *= x;
	for (int i = 0; i < (n); ++i) out[i] = in[-i & (n - 1)] - conj(in[i]);
	fft(out);
	for (int i = 0; i < ((int)(res).size()); ++i) res[i] = imag(out[i]) / (4 * n);
	return res;
}
