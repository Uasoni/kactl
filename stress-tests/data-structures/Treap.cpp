#include "../utilities/template.h"

#include "../../content/data-structures/Treap.h"

pair<TreapNode*, TreapNode*> split2(TreapNode* n, int v) {
	if (!n) return {};
	if (n->val >= v) {
		auto pa = split2(n->l, v);
		n->l = pa.second;
		n->recalc();
		return {pa.first, n};
	} else {
		auto pa = split2(n->r, v);
		n->r = pa.first;
		n->recalc();
		return {n, pa.second};
	}
}

int ra() {
	static unsigned x;
	x *= 4176481;
	x += 193861934;
	return x >> 1;
}

int main() {
	srand(3);
	for (int it = 0; it < (1000); ++it) {
		vector<TreapNode> nodes;
		vector<int> exp;
		for (int i = 0; i < (10); ++i) {
			nodes.emplace_back(i*2+2);
			exp.emplace_back(i*2+2);
		}
		TreapNode* n = 0;
		for (int i = 0; i < (10); ++i)
			n = merge(n, &nodes[i]);

		int v = rand() % 25;
		int left = cnt(split2(n, v).first);
		int rleft = (int)(lower_bound(begin(exp), end(exp), v) - exp.begin());
		assert(left == rleft);
	}

	for (int it = 0; it < (10000); ++it) {
		vector<TreapNode> nodes;
		vector<int> exp;
		for (int i = 0; i < (10); ++i) nodes.emplace_back(i);
		for (int i = 0; i < (10); ++i) exp.emplace_back(i);
		TreapNode* n = 0;
		for (int i = 0; i < (10); ++i)
			n = merge(n, &nodes[i]);

		int i = ra() % 11, j = ra() % 11;
		if (i > j) swap(i, j);
		if (i == j) continue;
		int k = ra() % 11;
		if (i < k && k < j) continue;

		move(n, i + 1, j, k + 1);
		// cerr << i << ' ' << j << ' ' << k << endl;

		int nk = (k >= j ? k - (j - i) : k);
		vector<int> iv(exp.begin() + i, exp.begin() + j);
		exp.erase(exp.begin() + i, exp.begin() + j);
		exp.insert(exp.begin() + nk, begin(iv), end(iv));

		int ind = 0;
		each(n, [&](int x) {
			// cerr << x << ' ';
			assert(x == exp[ind++]);
		});
		// cerr << endl;
	}
	cout<<"tests passed!"<<endl;
}
