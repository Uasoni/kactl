#pragma once

inline mt19937_64 stress_rng(0x6b6163746cULL);

// returns random int in [0, hi), like python's random.randrange
int rand_range(int hi) {
	assert(hi > 0);
	return uniform_int_distribution<int>(0, hi - 1)(stress_rng);
}

bool rand_bool() {
	return uniform_int_distribution<int>(0, 1)(stress_rng);
}

// returns random int in [lo, hi), like python's random.randrange
int rand_range(int lo, int hi) {
	return lo + rand_range(hi - lo);
}

// returns random int in [0, 2^64)
uint64_t rand_u64() {
	return stress_rng();
}

uint64_t rand_range(uint64_t hi) {
	assert(hi > 0);
	return uniform_int_distribution<uint64_t>(0, hi - 1)(stress_rng);
}

uint64_t rand_range(uint64_t lo, uint64_t hi) {
	return lo + rand_range(hi - lo);
}

int64_t rand_range(int64_t hi) {
	assert(hi > 0);
	return (int64_t) rand_range((uint64_t) hi);
}

int64_t rand_range(int64_t lo, int64_t hi) {
	return lo + rand_range(hi - lo);
}

// returns random int in [0, hi], like python's random.randint
int rand_incl(int hi) {
	return rand_range(hi + 1);
}

// returns random int in [lo, hi], like python's random.randint
int rand_incl(int lo, int hi) {
	return rand_range(lo, hi + 1);
}

int64_t rand_incl(int64_t hi) {
	return rand_range(hi + 1);
}

int64_t rand_incl(int64_t lo, int64_t hi) {
	return rand_range(lo, hi + 1);
}

// returns uniformly random double in [lo, hi)
double rand_double(double lo, double hi) {
	return uniform_real_distribution<double>(lo, hi)(stress_rng);
}

// int -> double based on IEEE 754 bitpattern
double bit_pattern_to_double(uint64_t x) {
	union {
		double d;
		uint64_t i;
	} u;
	u.i = x;
	return u.d;
}

// double -> int based on IEEE 754 bitpattern
uint64_t double_to_bit_pattern(double x) {
	union {
		double d;
		uint64_t i;
	} u;
	u.d = x;
	return u.i;
}

// random double
double rand_double_uniform_bit_pattern() {
	return bit_pattern_to_double(rand_u64());
}

// random double in [lo, hi), with any bit pattern being equally likely
double rand_double_uniform_bit_pattern(double lo, double hi) {
	return bit_pattern_to_double(rand_range(double_to_bit_pattern(lo), double_to_bit_pattern(hi)));
}

// add ~y ulps (units of last precision) to x, similar to calling next_after y times
double add_ulps(double x, int64_t y) {
	if (x == 0 && y < 0) {
		return -add_ulps(-x, -y);
	}
	return bit_pattern_to_double(double_to_bit_pattern(x) + y);
}

// random int in [-lim, lim], perturbed by a few ulps
double rand_near_int_ulps(int lim, int64_t ulps = 5) {
	return add_ulps(rand_incl(-lim, lim), rand_incl(-ulps, ulps));
}

// random int in [-lim, lim], perturbed by a random double in [-eps, eps]
double rand_near_int_eps(int lim, double eps) {
	return rand_incl(-lim, lim) + rand_double(-eps, eps);
}

template<class T>
void shuffle_vec(T& vec) {
	shuffle(begin(vec), end(vec), stress_rng);
}
