/**
 * Author: simon lindholm
 * Date: 2018-07-06
 * License: CC0
 * Description: permutation -> integer conversion. (not order preserving.)
 * integer -> permutation can use a lookup table.
 * Time: O(n)
 */
#pragma once

int perm_to_int(vector<int>& v) {
	int use = 0, i = 0, r = 0;
	for(int x:v) r = r * ++i + __builtin_popcount(use & -(1<<x)),
		use |= 1 << x;                     // (note: minus, not ~!)
	return r;
}
