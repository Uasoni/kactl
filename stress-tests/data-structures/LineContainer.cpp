#include "../utilities/template.h"

#include "../../content/data-structures/LineContainer.h"

namespace other {
// source: https://github.com/niklasb/contest-algos/blob/master/convex_hull/dynamic.cpp
const ll IS_QUERY = -(1LL<<62);
struct ReferenceLine {
	ll m, b;
	mutable function<const ReferenceLine*()> succ;
	bool operator<(const ReferenceLine& rhs) const {
		if (rhs.b != IS_QUERY) return m < rhs.m;
		const ReferenceLine* s = succ();
		if (!s) return 0;
		ll x = rhs.m;
		return b - s->b < (s->m - m) * x;
	}
};
struct DynamicHull : public multiset<ReferenceLine> { // will maintain upper hull for maximum
	bool bad(iterator y) {
		auto z = next(y);
		if (y == begin()) {
			if (z == end()) return 0;
			return y->m == z->m && y->b <= z->b;
		}
		auto x = prev(y);
		if (z == end()) return y->m == x->m && y->b <= x->b;
		return (x->b - y->b)*(z->m - y->m) >= (y->b - z->b)*(y->m - x->m);
	}
	void add(ll m, ll b) {
		auto y = insert({ m, b });
		y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
		if (bad(y)) { erase(y); return; }
		while (next(y) != end() && bad(next(y))) erase(next(y));
		while (y != begin() && bad(prev(y))) erase(prev(y));
	}
	ll query(ll x) {
		auto l = *lower_bound((ReferenceLine) { x, IS_QUERY });
		return l.m * x + l.b;
	}
};
}

int test2() {
	LineContainer mh;
	const int K = 10;
	ll x[K], v[K];
	for (int it = 0; it < (100); ++it) {
		mh.clear();
		int n = rand() % 100000 + 1;
		for (int j = 0; j < (K); ++j) x[j] = rand() % 1000 - 500, v[j] = LLONG_MIN;
// cerr << "---" << endl;
// cerr << x << endl;
		for (int i = 0; i < (n); ++i) {
			ll k = rand() % 100000 - 50000;
			ll m = rand() % (1LL << 30) - (1LL << 29);
// cerr << k << ' ' << m << endl;
			mh.add((int)k, (int)m);
			for (int j = 0; j < (K); ++j) v[j] = max(v[j], k*x[j] + m);
		}
// cerr << mh.eval(x) << ' ' << v << endl;
// for(auto &li: mh) cerr << li.k << ' ' << li.m << ' ' << li.p << endl;
		for (int j = 0; j < (K); ++j)
			assert(mh.query(x[j]) == v[j]);
	}
	return 0;
}

volatile ll glob;
int ra() {
	static unsigned blah;
	blah *= 12311231;
	blah += 129481762;
	return blah >> 1;
}

int main() {
	LineContainer mh;
	other::DynamicHull mh2;
	for (int it = 0; it < (10000000); ++it) {
		assert(mh.empty() == mh2.empty());
		int r = ra() % 100;
		if (r < 10) mh.clear(), mh2.clear();
		else if (r < 50) {
			int k = ra() % 10 - 5;
			int m = ra() % 100 - 50;
			mh.add(k, m);
			mh2.add(k, m);
		}
		else if (!mh.empty()) {
			int x = ra() % 10 - 5;
			assert(mh.query(x) == mh2.query(x));
		}
	}
	test2();
	cout<<"tests passed!"<<endl;
}
