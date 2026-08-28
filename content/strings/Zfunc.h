/**
 * Author: chilli
 * License: CC0
 * Description: z[i] computes the longest common prefix of the suffix starting
 *  at 1-indexed position i and the full string, except z[1] = 0. The returned
 *  vector has index 0 unused. (abacaba -> 0010301)
 * Time: O(n)
 * Status: stress-tested
 */
#pragma once

vector<int> z_function(const string& s) {
	vector<int> z((int)s.size());
	int l = -1, r = -1;
	for (int i = 1; i < (int)s.size(); ++i) {
		z[i] = i >= r ? 0 : min(r - i, z[i - l]);
		while (i + z[i] < (int)s.size() && s[i + z[i]] == s[z[i]])
			z[i]++;
		if (i + z[i] > r)
			l = i, r = i + z[i];
	}
	z.insert(begin(z), 0);
	return z;
}
