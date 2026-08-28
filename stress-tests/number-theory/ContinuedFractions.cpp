#include "../utilities/template.h"

#include "../../content/number-theory/ContinuedFractions.h"

int main() {
	for (int n = 1; n < (300); ++n) {
		vector<pair<double, pii>> v;
		for (int i = 0; i < (n+1); ++i) for (int j = 1; j < (n+1); ++j) if (__gcd(i,j) == 1) {
			double r = (double)i / j;
			v.emplace_back(r, pii(i,j));
		}
		v.emplace_back(1e9, pii(0,0));
		sort(begin(v), end(v));
		map<double, pii> actual(begin(v), end(v));

		for (int iter = 0; iter < (100000); ++iter) {
			double x = rand() / (RAND_MAX + 1.0) * 3;
			if (rand() % 2 == 0) x = (rand() % (3*n)) / (double)(rand() % (3*n) + 1);
			auto pa = approximate(x, n);
			auto it = actual.lower_bound(x), it2 = it;
			if (it2 != actual.begin()) --it2;
			auto best =
				min(make_pair(abs(it2->first - x), it2->second),
					make_pair(abs(it->first - x), it->second)).second;
			assert(best.first == pa.first);
			assert(best.second == pa.second);
		}
	}
	cout<<"tests passed!"<<endl;
	return 0;
}
