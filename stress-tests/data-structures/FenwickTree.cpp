#include "../utilities/template.h"

#include "../../content/data-structures/FenwickTree.h"

int main() {
	for (int it = 0; it < (100000); ++it) {
		int n = rand() % 10;
		FenwickTree tree(n);
		vector<int> values(n);
		for (int i = 1; i <= n; ++i) {
			int v = rand() % 3;
			tree.update(i, v);
			values[i - 1] += v;
		}
		int q = rand() % 20;
		int actual = tree.lower_bound(q);
		int expected = q <= 0 ? 0 : n + 1, sum = 0;
		for (int i = 1; i <= n && expected == n + 1; ++i) {
			sum += values[i - 1];
			if (sum >= q) expected = i;
		}
		assert(actual == expected);
	}
	cout<<"tests passed!"<<endl;
}
