/**
 * Author: user adamant on code_forces
 * Source: http://codeforces.com/blog/entry/12143
 * Description: for each 1-indexed position in a string, computes p[0][i] =
 *  half the longest even palindrome around i, and p[1][i] = the longest odd
 *  palindrome's half length rounded down. Both vectors have index 0 unused.
 * Time: O(N)
 * Status: stress-tested
 */
#pragma once

array<vector<int>, 2> manacher(const string& s) {
	int n = (int)(s).size();
	array<vector<int>,2> p = {vector<int>(n+1), vector<int>(n)};
	for (int z = 0; z < (2); ++z) for (int i=0,l=0,r=0; i < n; i++) {
		int t = r-i+!z;
		if (i<r) p[z][i] = min(t, p[z][l+t]);
		int left = i-p[z][i], right = i+p[z][i]-!z;
		while (left>=1 && right+1<n && s[left-1] == s[right+1])
			p[z][i]++, left--, right++;
		if (right>r) l=left, r=right;
	}
	array<vector<int>, 2> result = {vector<int>(n+1), vector<int>(n+1)};
	for (int z = 0; z < 2; ++z) for (int i = 0; i < n; ++i)
		result[z][i+1] = p[z][i];
	return result;
}
