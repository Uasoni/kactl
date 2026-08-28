#include "../utilities/template.h"

static unsigned random_state = 1231231;
int ra() {
	random_state *= 574841;
	random_state += 14;
	return random_state >> 1;
}

namespace maximum {

#include "../../content/data-structures/SegmentTree.h"

}

namespace nonabelian {

// https://en.wikipedia.org/wiki/dihedral_group_of_order_6
const int lut[6][6] = {
	{0, 1, 2, 3, 4, 5},
	{1, 0, 4, 5, 2, 3},
	{2, 5, 0, 4, 3, 1},
	{3, 4, 5, 0, 1, 2},
	{4, 3, 1, 2, 5, 0},
	{5, 2, 3, 1, 0, 4}
};

struct ReferenceSegmentTree {
	typedef int value_type;
	static constexpr value_type UNIT = 0;
	value_type f(value_type a, value_type b) { return lut[a][b]; }
	vector<value_type> s; int n;
	ReferenceSegmentTree(int n = 0, value_type def = 0) : s(2*n, def), n(n) {}
	void update(int pos, value_type val) {
		for (s[pos += n - 1] = val; pos > 1; pos /= 2)
			s[pos / 2] = f(s[pos & ~1], s[pos | 1]);
	}
	value_type query(int left, int right) {
		value_type left_result = UNIT, right_result = UNIT;
		for (left += n - 1, right += n; left < right;
				left /= 2, right /= 2) {
			if (left % 2) left_result = f(left_result, s[left++]);
			if (right % 2) right_result = f(s[--right], right_result);
		}
		return f(left_result, right_result);
	}
};

}

int main() {
	if (0) {
		const int N = 10000;
		maximum::SegmentTree tr(N);
		ll sum = 0;
		for (int it = 0; it < (1000000); ++it) {
			tr.update(ra() % N + 1, ra());
			int left = ra() % N + 1;
			int right = ra() % N + 1;
			if (left > right) swap(left, right);
			int v = tr.query(left, right);
			sum += v;
		}
		cout << sum << endl;
		// return 0;
	}

	for (int n = 1; n < (10); ++n) {
		maximum::SegmentTree tr(n);
		vector<int> v(n, maximum::SegmentTree::UNIT);
		for (int it = 0; it < (1000000); ++it) {
			int left = rand() % n + 1, right = rand() % n + 1;
			if (left > right) swap(left, right);
			int x = rand() % (n+2);

			int r = rand() % 100;
			if (r < 30) {
				int ma = tr.UNIT;
				for (int k = left; k <= right; ++k) ma = max(ma, v[k - 1]);
				assert(ma == tr.query(left, right));
			}
			else {
				tr.update(left, x);
				v[left - 1] = x;
			}
		}
	}

	for (int n = 1; n < (10); ++n) {
		nonabelian::ReferenceSegmentTree tr(n);
		vector<int> v(n);
		for (int it = 0; it < (1000000); ++it) {
			int left = rand() % n + 1, right = rand() % n + 1;
			if (left > right) swap(left, right);
			int x = rand() % 6;

			int r = rand() % 100;
			if (r < 30) {
				int ma = tr.UNIT;
				for (int k = left; k <= right; ++k)
					ma = nonabelian::lut[ma][v[k - 1]];
				assert(ma == tr.query(left, right));
			}
			else {
				tr.update(left, x);
				v[left - 1] = x;
			}
		}
	}
	cout<<"tests passed!"<<endl;
}
