#include "../utilities/template.h"

namespace dynamic {
vector<int> eratosthenes(int limit) {
	const int S = (int)round(sqrt(limit)), R = limit / 2;
	vector<int> pr({2}), sieve(S + 1); pr.reserve(limit / (int)log(limit));
	vector<array<int, 2>> cp;
	for (int i = 3; i <= S; i += 2) if (!sieve[i]) {
		cp.push_back({i, i * i / 2});
		for (int j = i * i; j <= S; j += 2 * i) sieve[j] = 1;
	}
	for (int left = 1; left <= R; left += S) {
		vector<bool> block(S);
		// array<bool, S> block{};
		for (auto &[p, idx] : cp)
			for (int i=idx; i < S+left; idx = (i+=p)) block[i-left] = 1;
		for (int i = 0; i < (min(S, R - left)); ++i)
			if (!block[i]) pr.push_back((left + i) * 2 + 1);
	}
	return pr;
}
}
#include "../../content/number-theory/FastEratosthenes.h"
#include "../../content/number-theory/Eratosthenes.h"


int main() {
	vector<int> pr1 = eratosthenes_sieve(limit);
	vector<int> pr2 = eratosthenes();
	assert(pr1 == pr2);

	for (int lim=121; lim<1000; lim++) {
		vector<int> pr = eratosthenes_sieve(lim);
		vector<int> r = dynamic::eratosthenes(lim);
		assert(pr == r);
	}
	cout<<"tests passed!"<<endl;
}
