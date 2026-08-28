/**
 * Author: 罗穗骞, chilli
 * Date: 2019-04-11
 * License: unknown
 * Source: suffix array - a powerful tool for dealing with strings
 * (chinese IOI national team training paper, 2009)
 * Description: builds suffix array for a string.
 * \texttt{sa[i]} is the 1-indexed starting position of the suffix which
 * is $i$'th in the sorted suffix array.
 * the returned vectors have size $n+2$ with index 0 unused, and
 * \texttt{sa[1] = n+1} is the empty suffix.
 * the \texttt{lcp} array contains longest common prefixes for
 * neighbouring strings in the suffix array:
 * \texttt{lcp[i] = lcp(sa[i], sa[i-1])}, \texttt{lcp[0] = 0}.
 * the input string must not contain any nul chars.
 * Time: O(n \log n)
 * Status: stress-tested
 */
#pragma once

struct SuffixArray {
	vector<int> sa, lcp;
	SuffixArray(string s, int lim=256) { // or vector<int>
		s.push_back(0); int n = (int)(s).size(), k = 0, a, b;
		vector<int> x(begin(s), end(s)), y(n), ws(max(n, lim));
		sa = lcp = y, iota(begin(sa), end(sa), 0);
		for (int j = 0, p = 0; p < n; j = max(1, j * 2), lim = p) {
			p = j, iota(begin(y), end(y), n - j);
			for (int i = 0; i < (n); ++i) if (sa[i] >= j) y[p++] = sa[i] - j;
			fill(begin(ws), end(ws), 0);
			for (int i = 0; i < (n); ++i) ws[x[i]]++;
			for (int i = 1; i < (lim); ++i) ws[i] += ws[i - 1];
			for (int i = n; i--;) sa[--ws[x[y[i]]]] = y[i];
			swap(x, y), p = 1, x[sa[0]] = 0;
			for (int i = 1; i < (n); ++i) a = sa[i - 1], b = sa[i], x[b] =
				(y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
		}
		for (int i = 0, j; i < n - 1; lcp[x[i++]] = k)
			for (k && k--, j = sa[x[i] - 1];
					s[i + k] == s[j + k]; k++);
		sa.insert(begin(sa), 0); lcp.insert(begin(lcp), 0);
		for (int i = 1; i < (int)sa.size(); ++i) ++sa[i];
	}
};
