/**
 * Author: lukas polacek
 * Date: 2009-09-28
 * License: CC0
 * Source: folklore
 * Description: operators for modular arithmetic. you need to set {\tt ModInt} to
 * some number first and then you can use the structure.
 */
#pragma once

#include "euclid.h"

const ll MOD = 17; // change to something else
struct ModInt {
	ll x;
	ModInt(ll xx) : x(xx) {}
	ModInt operator+(ModInt b) { return ModInt((x + b.x) % MOD); }
	ModInt operator-(ModInt b) { return ModInt((x - b.x + MOD) % MOD); }
	ModInt operator*(ModInt b) { return ModInt((x * b.x) % MOD); }
	ModInt operator/(ModInt b) { return *this * invert(b); }
	ModInt invert(ModInt a) {
		ll x, y, g = euclid(a.x, MOD, x, y);
		assert(g == 1); return ModInt((x + MOD) % MOD);
	}
	ModInt operator^(ll e) {
		if (!e) return ModInt(1);
		ModInt r = *this ^ (e / 2); r = r * r;
		return e&1 ? *this * r : r;
	}
};
