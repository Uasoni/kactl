#include "../utilities/template.h"

#include "../../content/various/FastKnapsack.h"

int naive(vector<int> w, int t) {
	vector<bool> can_reach(t+1);
	can_reach[0] = true;
	for (int x : w) {
		for (int i = t-x; i >= 0; --i) {
			if (can_reach[i]) can_reach[i+x] = true;
		}
	}
	for (int i = t;; i--)
		if (can_reach[i]) return i;
	assert(false);
}

int main() {
	const int MAX_N = 10;
	const int MAX_W = 10;
	const int iters = 1000000;
	for (int it = 0; it < (iters); ++it) {
		int n = rand() % MAX_N;
		int maxw = rand() % MAX_W + 1;
		vector<int> w(n);
		for (int i = 0; i < (n); ++i)
			w[i] = rand()%(maxw+1);
		int t = rand() % (MAX_N*maxw);
		assert(naive(w,t) == knapsack(w,t));
	}
	cout<<"tests passed!"<<endl;
}
