/**
 * Author: johan sannemo
 * Date: 2016-12-15
 * License: CC0
 * Description: pi(s)[x] computes the length of the longest proper prefix of s
 *  ending at 1-indexed position x. The returned vector has a dummy at index 0.
 * can be used to find all occurrences of a string.
 * Time: O(n)
 * Status: tested on kattis:stringmatching
 */
#pragma once

vector<int> pi(const string& s) {
	vector<int> p(s.size() + 1);
	for (int i = 2; i <= (int)s.size(); ++i) {
		int g = p[i-1];
		while (g && s[i-1] != s[g]) g = p[g];
		p[i] = g + (s[i-1] == s[g]);
	}
	return p;
}

vector<int> match(const string& s, const string& pat) {
	vector<int> p = pi(pat + '\0' + s), res;
	for (int i = (int)p.size()-(int)s.size(); i < (int)p.size(); ++i)
		if (p[i] == (int)(pat).size()) res.push_back(i - 2 * (int)(pat).size());
	return res;
}
