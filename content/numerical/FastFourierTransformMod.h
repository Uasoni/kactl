/**
 * Author: chilli
 * Date: 2019-04-25
 * License: CC0
 * Source: http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf
 * Description: higher precision FFT, can be used for convolutions modulo arbitrary integers
 * as long as $N\log_2 N\cdot \text{mod} < 8.6 \cdot 10^{14}$ (in practice $10^{16}$ or higher).
 * inputs must be in $[0, \text{mod})$.
 * Time: O(N \log N), where $N = |A|+|B|$ (twice as slow as NTT or FFT)
 * Status: stress-tested
 * Details: an in-depth examination of precision for both FFT and fft_mod can be found
 * here (https://github.com/simonlindholm/fft-precision/blob/master/fft-precision.md)
 */
#pragma once

#include "FastFourierTransform.h"
template<int MODULUS> vector<ll> conv_mod(const vector<ll> &a, const vector<ll> &b) {
	if (a.empty() || b.empty()) return {};
	vector<ll> res((int)(a).size() + (int)(b).size() - 1);
	int log_size=32-__builtin_clz((int)(res).size()), n=1<<log_size;
	int cut=int(sqrt(MODULUS));
	vector<C> left(n), right(n), outs(n), outl(n);
	for (int i = 0; i < ((int)(a).size()); ++i) left[i] = C((int)a[i] / cut, (int)a[i] % cut);
	for (int i = 0; i < ((int)(b).size()); ++i) right[i] = C((int)b[i] / cut, (int)b[i] % cut);
	fft(left), fft(right);
	for (int i = 0; i < (n); ++i) {
		int j = -i & (n - 1);
		outl[j] = (left[i] + conj(left[j])) * right[i] / (2.0 * n);
		outs[j] = (left[i] - conj(left[j])) * right[i] / (2.0 * n) / 1i;
	}
	fft(outl), fft(outs);
	for (int i = 0; i < ((int)(res).size()); ++i) {
		ll av = ll(real(outl[i])+.5), cv = ll(imag(outs[i])+.5);
		ll bv = ll(imag(outl[i])+.5) + ll(real(outs[i])+.5);
		res[i] = ((av % MODULUS * cut + bv) % MODULUS * cut + cv) % MODULUS;
	}
	return res;
}
