/**
 * Author: simon lindholm
 * Date: 2015-03-15
 * License: CC0
 * Source: own work
 * Description: self-explanatory methods for string hashing.
 * Status: stress-tested
 */
#pragma once

// arithmetic mod 2^64-1. 2x slower than mod 2^64 and more
// code, but works on evil test data (e.g. thue-morse, where
// ABBA... and BAAB... of length 2^10 hash the same mod 2^64).
// "typedef ull HashValue;" instead if you think test data is random,
// or work mod 10^9+7 if the birthday paradox is not a problem.
typedef uint64_t ull;
struct HashValue {
	ull x; HashValue(ull x=0) : x(x) {}
	HashValue operator+(HashValue o) { return x + o.x + (x + o.x < x); }
	HashValue operator-(HashValue o) { return *this + ~o.x; }
	HashValue operator*(HashValue o) { auto m = (__uint128_t)x * o.x;
		return HashValue((ull)m) + (ull)(m >> 64); }
	ull get() const { return x + !~x; }
	bool operator==(HashValue o) const { return get() == o.get(); }
	bool operator<(HashValue o) const { return get() < o.get(); }
};
static const HashValue BASE = (ll)1e11+3;

struct RollingHash {
	vector<HashValue> hashes, powers;
	RollingHash(const string& str) : hashes((int)str.size()+1), powers(hashes) {
		powers[0] = 1;
		for (int i = 0; i < ((int)(str).size()); ++i)
			hashes[i+1] = hashes[i] * BASE + str[i],
			powers[i+1] = powers[i] * BASE;
	}
	HashValue query(int left, int right) { // 1-indexed [left, right]
		return hashes[right] - hashes[left - 1] * powers[right - left + 1];
	}
};

vector<HashValue> get_hashes(const string& str, int length) {
	if ((int)str.size() < length) return vector<HashValue>(1);
	HashValue hash = 0, power = 1;
	for (int i = 0; i < (length); ++i)
		hash = hash * BASE + str[i], power = power * BASE;
	vector<HashValue> result = {HashValue{}, hash};
	for (int i = length; i < ((int)(str).size()); ++i) {
		result.push_back(hash = hash * BASE + str[i] - power * str[i-length]);
	}
	return result;
}

HashValue hash_string(const string& s) {
	HashValue hash{};
	for (char c : s) hash = hash * BASE + c;
	return hash;
}
