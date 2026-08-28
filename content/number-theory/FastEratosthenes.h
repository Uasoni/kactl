/**
 * Author: jakob kogler, chilli, pajenegod
 * Date: 2020-04-12
 * License: CC0
 * Description: prime sieve for generating all primes smaller than LIM.
 * Time: LIM=1e9 $\approx$ 1.5s
 * Status: stress-tested
 * Details: despite its n log log n complexity, segmented sieve is still faster
 * than other options, including bitset sieves and linear sieves. this is
 * primarily due to its low memory usage, which reduces cache misses. this
 * implementation skips even numbers.
 *
 * benchmark can be found here: https://ideone.com/e7_tb_x4
 *
 * the line `for (int i=idx; i<S+L; idx = (i += p))` is done on purpose for performance reasons.
 * se https://github.com/kth-competitive-programming/kactl/pull/166#discussion_r408354338
 */
#pragma once

const int LIM = 1e6;
bitset<LIM> is_prime;
vector<int> eratosthenes() {
	const int block_size = (int)round(sqrt(LIM)), half_limit = LIM / 2;
	vector<int> pr = {2}, sieve(block_size+1); pr.reserve(int(LIM/log(LIM)*1.1));
	vector<pii> cp;
	for (int i = 3; i <= block_size; i += 2) if (!sieve[i]) {
		cp.push_back({i, i * i / 2});
		for (int j = i * i; j <= block_size; j += 2 * i) sieve[j] = 1;
	}
	for (int block_left = 1; block_left <= half_limit; block_left += block_size) {
		array<bool, block_size> block{};
		for (auto &[p, idx] : cp)
			for (int i=idx; i < block_size+block_left; idx = (i+=p)) block[i-block_left] = 1;
		for (int i = 0; i < min(block_size, half_limit - block_left); ++i)
			if (!block[i]) pr.push_back((block_left + i) * 2 + 1);
	}
	for (int i : pr) is_prime[i] = 1;
	return pr;
}
