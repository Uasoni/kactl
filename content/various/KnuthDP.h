/**
 * Author: simon lindholm
 * License: CC0
 * Source: http://codeforces.com/blog/entry/8219
 * Description: when doing DP on intervals: $a[i][j] = \min_{i < k < j}(a[i][k] + a[k][j]) + f(i, j)$, where the (minimal) optimal $k$ increases with both $i$ and $j$,
 *  one can solve intervals in increasing order of length, and search $k = p[i][j]$ for $a[i][j]$ only between $p[i][j-1]$ and $p[i+1][j]$.
 *  this is known as knuth DP. sufficient criteria for this are if $f(b,c) \le f(a,d)$ and $f(a,c) + f(b,d) \le f(a,d) + f(b,c)$ for all $a \le b \le c \le d$.
 *  consider also: LineContainer (ch. data structures), monotone queues, ternary search.
 * Time: O(N^2)
 */
