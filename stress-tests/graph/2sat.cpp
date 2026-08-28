#include "../utilities/template.h"
#include "../utilities/random.h"

#include "../../content/graph/2sat.h"

int main1() {
	const int N = 100000, M = 10000000;
	// random constraints, unsolvable
	{
		TwoSat ts(N);
		for (int i = 0; i < (M); ++i) {
			int r = rand();
			int s = r;
			r >>= 2;
			int a = r % N;
			r >>= 5;
			int b = r % N;
			if (a == b) continue;
			ts.either(s & 1 ? a + 1 : -(a + 1), s & 2 ? b + 1 : -(b + 1));
		}
		assert(ts.solve() == 0);
	}
	// random solvable instance
	{
		vector<bool> v(N);
		for (int i = 0; i < (N); ++i) v[i] = rand() & (1 << 20);
		TwoSat ts(N);
		for (int i = 0; i < (M); ++i) {
			int r = rand();
			int s = r;
			r >>= 2;
			int a = r % N;
			r >>= 5;
			int b = r % N;
			if (a == b) continue;
			ts.either(v[a] ? a + 1 : -(a + 1), s & 1 ? b + 1 : -(b + 1));
		}
		assert(ts.solve() == 1);
	}
	return 0;
}

int main2() {
	int N = 4;
	TwoSat ts(N);
	ts.either(1, 2);
	ts.either(1, -2);
	ts.either(-3, -4);
	assert(ts.solve()==1);
	assert(ts.values == vector<int>({-1, 1, 1, 0, 0}));
	return 0;
}

int ra() {
	static unsigned X;
	X *= 1283611;
	X += 123;
	return X >> 1;
}

// test at_most_one
int main() {
	main1();
	main2();
	const int N = 100, M = 400;
	for (int it = 0; it < (100); ++it) {
		vector<bool> v(N);
		for (int i = 0; i < (N); ++i) v[i] = ra() & (1 << 20);
		TwoSat ts(N);
		vector<vector<int>> atm;
		vector<int> r;
		for (int i = 0; i < (M); ++i) {
			if (ra()%100 < 5) {
				int r = ra();
				int s = r;
				r >>= 2;
				int a = r % N;
				r >>= 5;
				int b = r % N;
				if (a == b) continue;
				ts.either(v[a] ? a + 1 : -(a + 1), (s&1) ? b + 1 : -(b + 1));
			} else {
				int k = ra() % 4 + 1;
				r.clear();
				for (int ki = 0; ki < (k-1); ++ki) {
					int a = ra() % N;
					r.push_back(v[a] ? -(a + 1) : a + 1);
				}
				int variable = ra() % N + 1;
				r.push_back(ra() & 1 ? variable : -variable);
				shuffle(begin(r), end(r), stress_rng);
				ts.at_most_one(r);
				atm.push_back(r);
			}
		}
		assert(ts.solve());
		int to = 0;
		for (int i = 0; i < N; ++i) to += (ts.values[i + 1] == v[i]);
		for(auto &r: atm) {
			int co = 0;
			for(auto &x: r) co += (ts.values[abs(x)] == (x > 0));
			assert(co <= 1);
		}
	}
	cout<<"tests passed!"<<endl;
	return 0;
}
