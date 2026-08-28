#include "../utilities/template.h"

#include "../../content/various/FastMod.h"

typedef unsigned long long ull;
struct OldBarrett {
	ull b, m;
	OldBarrett(ull b) : b(b), m(-1ULL / b) {}
	ull reduce(ull a) {
		ull q = (ull)((__uint128_t(m) * a) >> 64), r = a - q * b;
		return r >= b ? r - b : r;
	}
};

constexpr int PARALLEL_VALUES = 1;
constexpr int BENCHMARK_MODULUS = 90'217'093;

template<class Reduce>
void run_benchmark(int modulus, Reduce reduce) {
	array<ll, PARALLEL_VALUES> results;
	iota(begin(results), end(results), 1);
	for (int value = 1; value < modulus; ++value)
		for (ll& result : results) result = reduce(result, value);
	for (ll result : results) cout << result << '\n';
}

void perf_plain(int modulus) {
	run_benchmark(modulus, [&](ll result, int value) {
		return result * value % modulus;
	});
}

template<int MODULUS>
void perf_const() {
	run_benchmark(MODULUS, [](ll result, int value) {
		return result * value % MODULUS;
	});
}

void perf_old_barrett(int modulus) {
	OldBarrett barrett(modulus);
	run_benchmark(modulus, [&](ll result, int value) {
		return (ll)barrett.reduce((ull)result * value);
	});
}

void perf_barrett(int modulus) {
	FastMod barrett(modulus);
	run_benchmark(modulus, [&](ll result, int value) {
		return (ll)barrett.reduce((ull)result * value);
	});
}

ull rand_u64() {
	ull ret = rand();
	ret <<= 23;
	ret ^= rand();
	ret <<= 23;
	ret ^= rand();
	return ret;
}

void test_correctness() {
	const int bflim = 3000;
	for (int a = 0; a < (bflim); ++a) for (int b = 2; b < (bflim); ++b) {
		FastMod bar(b);
		ull ret = bar.reduce(a);
		assert((ret == 0) == (a == 0));
		if (ret >= (ull)b) ret -= b;
		assert(ret == (ull)(a % b));
	}
	for (int it = 0; it < (10'000'000); ++it) {
		ull a = rand_u64();
		ull b = rand_u64();
		if (b == 0) continue;
		FastMod bar(b);
		ull ret = bar.reduce(a);
		if (ret >= b) ret -= b;
		assert(ret == a % b);
	}
	cout<<"tests passed!"<<endl;
}

int main(int argc, char** argv) {
	if (argc == 1) {
		test_correctness();
		return 0;
	}
	int which = atoi(argv[1]);
	if (which == 0) perf_plain(BENCHMARK_MODULUS);
	if (which == 1) perf_const<BENCHMARK_MODULUS>();
	if (which == 2) perf_old_barrett(BENCHMARK_MODULUS);
	if (which == 3) perf_barrett(BENCHMARK_MODULUS);
	return 0;
}
