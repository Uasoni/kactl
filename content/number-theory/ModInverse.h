/**
 * Author: simon lindholm
 * Date: 2016-07-24
 * License: CC0
 * Source: russian page
 * Description: pre-computation of modular inverses. assumes LIM $\le$ mod and that mod is a prime.
 * Status: works
 */
#pragma once

// const ll mod = 1000000007, LIM = 200000; ///include-line
ll* inv = new ll[LIM] - 1; inv[1] = 1;
for (int i = 2; i < (LIM); ++i) inv[i] = mod - (mod / i) * inv[mod % i] % mod;
