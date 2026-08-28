/**
 * Author: simon lindholm, chilli
 * Date: 2018-07-23
 * License: CC0
 * Source: http://codeforces.com/blog/entry/60737
 * Description: hash map with mostly the same API as unordered\_map, but about
 *  3x faster. Uses GNU policy-based data structures and a randomized hash.
 */
#pragma once

struct CustomHash {
	static uint64_t split_mix64(uint64_t x) {
		x += 0x9e3779b97f4a7c15ULL;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
		x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
		return x ^ (x >> 31);
	}
	size_t operator()(uint64_t x) const {
		static const uint64_t RANDOM = chrono::steady_clock::now()
			.time_since_epoch().count();
		return split_mix64(x + RANDOM);
	}
};
#include <ext/pb_ds/assoc_container.hpp> /** keep-include */

__gnu_pbds::gp_hash_table<ll, int, CustomHash> h;
