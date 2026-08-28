#include "../utilities/template.h"

#include "../../content/numerical/FastSubsetTransform.h"

int main() {
	for (int k = 0; k < (10); ++k) {
		vector<int> a(1 << k), b = a, c = a, target = a;
		for(auto &x: a) x = rand() % 6 - 2;
		for(auto &x: b) x = rand() % 6 - 2;
		for (int i = 0; i < (1 << k); ++i) for (int j = 0; j < (1 << k); ++j) target[i & j] += a[i] * b[j];
		// for (int i = 0; i < (1 << k); ++i) cout << a[i] << ' '; cout << endl;
		// for (int i = 0; i < (1 << k); ++i) cout << b[i] << ' '; cout << endl;
		fast_subset_transform(a, false);
		fast_subset_transform(b, false);
		for (int i = 0; i < (1 << k); ++i) c[i] = a[i] * b[i];
		fast_subset_transform(c, true);
		// for (int i = 0; i < (1 << k); ++i) cout << c[i] << ' '; cout << endl;
		// for (int i = 0; i < (1 << k); ++i) cout << target[i] << ' '; cout << endl;
		assert(c == target);
	}
	cout<<"tests passed!"<<endl;
}
