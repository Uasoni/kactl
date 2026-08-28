/**
 * Author: simon lindholm
 * Date: 2015-03-15
 * License: CC0
 * Source: own work
 * Description: various self-explanatory methods for string hashing.
 * use on codeforces, which lacks 64-bit support and where solutions can be hacked.
 * Status: stress-tested
 */
#pragma once

typedef uint64_t ull;
static int BASE; // initialized below

// arithmetic mod two primes and 2^32 simultaneously.
// "typedef uint64_t HashValue;" instead if thue-morse does not apply.
template<int MOD, class B>
struct HashComponent {
	int x; B b; HashComponent(int x=0) : x(x), b(x) {}
	HashComponent(int x, B b) : x(x), b(b) {}
	HashComponent operator+(HashComponent o) {
		int y = x + o.x; return {y - (y >= MOD) * MOD, b + o.b}; }
	HashComponent operator-(HashComponent o) {
		int y = x - o.x; return {y + (y < 0) * MOD, b - o.b}; }
	HashComponent operator*(HashComponent o) {
		return {(int)(1LL * x * o.x % MOD), b * o.b}; }
	explicit operator ull() const { return x ^ (ull) b << 21; }
	bool operator==(HashComponent o) const { return (ull)*this == (ull)o; }
	bool operator<(HashComponent o) const { return (ull)*this < (ull)o; }
};
typedef HashComponent<1000000007,
	HashComponent<1000000009, unsigned>> HashValue;

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
