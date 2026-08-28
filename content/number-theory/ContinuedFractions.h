/**
 * Author: simon lindholm
 * Date: 2018-07-15
 * License: CC0
 * Source: wikipedia
 * Description: given $N$ and a real number $x \ge 0$, finds the closest rational approximation $p/q$ with $p, q \le N$.
 * it will obey $|p/q - x| \le 1/q_n$.
 *
 * for consecutive convergents, $p_{k+1}q_k - q_{k+1}p_k = (-1)^k$.
 * ($p_k/q_k$ alternates between $>x$ and $<x$.)
 * if $x$ is rational, $y$ eventually becomes $\infty$;
 * if $x$ is the root of a degree $2$ polynomial the $a$'s eventually become cyclic.
 * Time: O(\log N)
 * Status: stress-tested for n <= 300
 */

typedef double d; // for N ~ 1e7; long double for N ~ 1e9
pair<ll, ll> approximate(d x, ll max_denominator) {
	ll last_p = 0, last_q = 1, p = 1, q = 0, inf = LLONG_MAX; d y = x;
	for (;;) {
		ll lim = min(p ? (max_denominator-last_p) / p : inf,
			q ? (max_denominator-last_q) / q : inf),
		   a = (ll)floor(y), b = min(a, lim),
		   next_p = b*p + last_p, next_q = b*q + last_q;
		if (a > b) {
			// if b > a/2, we have a semi-convergent that gives us a
			// better approximation; if b = a/2, we *may* have one.
			// return {p, q} here for a more canonical approximation.
			return (abs(x - (d)next_p / (d)next_q) < abs(x - (d)p / (d)q)) ?
				make_pair(next_p, next_q) : make_pair(p, q);
		}
		if (abs(y = 1/(y - (d)a)) > 3*max_denominator) {
			return {next_p, next_q};
		}
		last_p = p; p = next_p;
		last_q = q; q = next_q;
	}
}
