#include "../utilities/template.h"

#include "../../content/data-structures/LazySegmentTree.h"

static unsigned random_state;
int ra() {
	random_state *= 791231;
	random_state += 1231;
	return (int)(random_state >> 1);
}

volatile int res;
int main() {
	int n = 10;
	vector<int> v(n);
	iota(begin(v), end(v), 0);
	mt19937 rng(17);
	shuffle(begin(v), end(v), rng);
	LazySegmentTree tree(v);
	for (int left = 1; left <= n; ++left)
		for (int right = left; right <= n; ++right) {
		int expected = -INF;
		for (int k = left; k <= right; ++k)
			expected = max(expected, v[k - 1]);
		assert(expected == tree.query(left, right));
	}
	for (int it = 0; it < (1000000); ++it) {
		int left = ra() % n + 1, right = ra() % n + 1;
		if (left > right) swap(left, right);
		int x = (ra() % 10) - 5;

		int r = ra() % 100;
		if (r < 30) {
			::res = tree.query(left, right);
			int ma = -INF;
			for (int k = left; k <= right; ++k) ma = max(ma, v[k - 1]);
			assert(ma == ::res);
		}
		else if (r < 70) {
			tree.add(left, right, x);
			for (int k = left; k <= right; ++k) v[k - 1] += x;
		}
		else {
			tree.set(left, right, x);
			for (int k = left; k <= right; ++k) v[k - 1] = x;
		}
	}
	cout<<"tests passed!"<<endl;
}
