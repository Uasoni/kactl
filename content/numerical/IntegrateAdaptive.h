/**
 * Author: simon lindholm
 * Date: 2015-02-11
 * License: CC0
 * Source: wikipedia
 * Description: fast integration using an adaptive simpson's rule.
 * Usage:
	double sphere_volume = integrate(-1, 1, [](double x) {
	return integrate(-1, 1, [\&](double y) {
	return integrate(-1, 1, [\&](double z) {
	return x*x + y*y + z*z < 1; });});});
 * Status: mostly untested
 */
#pragma once

typedef double d;

template <class F>
d integrate_rec(F& f, d a, d b, d eps, d estimate) {
	auto simpson = [&](d left, d right) {
		return (f(left) + 4 * f((left + right) / 2) + f(right))
			* (right - left) / 6;
	};
	d c = (a + b) / 2;
	d left_estimate = simpson(a, c), right_estimate = simpson(c, b);
	d refined = left_estimate + right_estimate;
	if (abs(refined - estimate) <= 15 * eps || b - a < 1e-10)
		return refined + (refined - estimate) / 15;
	return integrate_rec(f, a, c, eps / 2, left_estimate)
		+ integrate_rec(f, c, b, eps / 2, right_estimate);
}
template<class F>
d integrate(d a, d b, F f, d eps = 1e-8) {
	d estimate = (f(a) + 4 * f((a + b) / 2) + f(b)) * (b - a) / 6;
	return integrate_rec(f, a, b, eps, estimate);
}
