/**
 * Author: stjepan glavina
 * License: unlicense
 * Source: https://github.com/stjepang/snippets/blob/master/min_rotation.cpp
 * Description: finds the lexicographically smallest rotation of a string.
 * Time: O(N)
 * Usage:
 *  rotate(v.begin(), v.begin()+min_rotation(v)-1, v.end());
 * Status: stress-tested
 */
#pragma once

int min_rotation(string s) {
	int a=0, n=(int)(s).size(); s += s;
	for (int b = 0; b < n; ++b) for (int k = 0; k < n; ++k) {
		if (a+k == b || s[a+k] < s[b+k]) {b += max(0, k-1); break;}
		if (s[a+k] > s[b+k]) { a = b; break; }
	}
	return a + 1;
}
