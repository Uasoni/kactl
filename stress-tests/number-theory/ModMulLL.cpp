#include "../utilities/template.h"

#include "../../content/number-theory/ModMulLL.h"

const int ITERS = 5'000'000; // (not really enough to say much, need >1e10 for any kind of certainty)

ull double_modmul(ull a, ull b, ull mod) {
	ll ret = a * b - mod * ull(1. / (double)mod * (double)a * (double)b);
	return ret + mod * (ret < 0) - mod * (ret >= (ll)mod);
}

ull int128_modmul(ull a, ull b, ull m) { return (ull)((__uint128_t)a * b % m); }

void test(ull lim, bool expect_success, bool use_doubles) {
	mt19937_64 rng(1);
	uniform_int_distribution<ull> uni(1, lim);
	uniform_int_distribution<ull> uni_small(0, lim / 10000);

	int it = 0;
	for (int i = 0;; i++) {
		// if (i % 1'000'000 == 0) cerr << '.' << flush;
		ull c = i&1 ? lim - uni_small(rng) : uni(rng);
		ull a = i&2 ? c - uni_small(rng) : i&4 && !use_doubles ? (1ULL << 62) - uni_small(rng) : uni(rng);
		ull b = i&8 ? c - uni_small(rng) : uni(rng);
		if (a > c || b > c) continue;
		if (expect_success && it++ >= ITERS) break;
		ull l = int128_modmul(a, b, c);
		ull r = use_doubles ? double_modmul(a, b, c) : mod_mul(a, b, c);
		if (l != r) {
			if (!expect_success) break;
			cout << a << ' ' << b << ' ' << c << endl;
			cout << l << ' ' << r << endl;
			abort();
		}
	}
}

void test_sq(ull lim, bool expect_success, bool use_doubles) {
	// test that modmul works for squaring slightly beyond the stated bounds.
	// factor.h relies on this (and has a proof sketch in the doc comment).
	mt19937_64 rng(1);
	uniform_int_distribution<ull> uni(1, lim);
	uniform_int_distribution<ull> uni_small(0, lim / 10000);
	uniform_int_distribution<ull> uni_tiny(0, (int)(sqrt(lim) / 2));

	for (int i = 0;; i++) {
		if (expect_success && i >= ITERS) break;
		// if (i % 1'000'000 == 0) cerr << '.' << flush;
		ull c = i&1 ? lim - uni_small(rng) : uni(rng);
		ull a = expect_success ? c + uni_tiny(rng) : c + uni_small(rng);
		ull l = int128_modmul(a, a, c);
		ull r = use_doubles ? double_modmul(a, a, c) : mod_mul(a, a, c);
		if (l != r) {
			if (!expect_success) break;
			cout << a << ' ' << c << endl;
			cout << l << ' ' << r << endl;
			abort();
		}
	}
}

int main() {
	const ull lim_doubles = 1ULL << 52;
	test(lim_doubles, true, true);
	test((ull)(lim_doubles * 1.02L), false, true);

	test_sq(lim_doubles, true, true);

	const ull lim = 7268172458553106874ULL; // floor((sqrt(177) - 7) / 16 * 2**64)
	test(lim, true, false);
	test((ull)(lim * 1.01L), false, false);
	// test((ull)(lim * 1.001L), false, false);

	test_sq(lim, true, false);
	test_sq(lim, false, false);
	cout << "tests passed!" << endl;
}
