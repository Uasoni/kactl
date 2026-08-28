#include "../utilities/template.h"

#include "../../content/data-structures/RMQ.h"

int main() {
	srand(2);
	for (int n = 0; n < (100); ++n) {
		vector<int> v(n);
		for (int i = 0; i < (n); ++i) v[i] = i;
		mt19937 rng(2 + n);
		shuffle(begin(v), end(v), rng);
		RangeMinimumQuery<int> range_minimum(v);
		for (int left = 1; left <= n; ++left)
			for (int right = left; right <= n; ++right) {
			int actual = range_minimum.query(left, right);
			int expected = 1 << 29;
			for (int k = left; k <= right; ++k)
				expected = min(expected, v[k - 1]);
			assert(expected == actual);
		}
	}
	cout<<"tests passed!"<<endl;
}
