#include "../utilities/template.h"

ll det(vector<vector<ll>>& a) {
	int n = (int)(a).size(); ll ans = 1;
	for (int i = 0; i < (n); ++i) {
		for (int j = i+1; j < (n); ++j) {
			while (a[j][i] != 0) { // gcd step
				ll t = a[i][i] / a[j][i];
				if (t) for (int k = i; k < (n); ++k)
					a[i][k] = (a[i][k] - a[j][k] * t);
				swap(a[i], a[j]);
				ans *= -1;
			}
		}
		ans = ans * a[i][i];
		if (!ans) return 0;
	}
	return ans;
}

ll rec(vector<vector<int>>& ed, int active, int finished) {
	int n = (int)(ed).size();
	if (finished == (1 << n) - 1) return 1;
	int chosen = -1;
	for (int i = 0; i < (n); ++i) if (!(finished & (1 << i)) && (active & (1 << i))) {
		chosen = i;
		break;
	}
	if (chosen == -1) return 0; // missing nodes from tree
	int si = (int)(ed[chosen]).size();
	ll res = 0;
	for (int bi = 0; bi < ((1 << si)); ++bi) {
		int edto = 0;
		for (int i = 0; i < (si); ++i) {
			if (bi & (1 << i)) {
				if (edto & (1 << ed[chosen][i])) goto fail;
				edto |= 1 << ed[chosen][i];
			}
		}
		if (active & edto) continue;
		res += rec(ed, active | edto, finished | (1 << chosen));
fail:;
	}
	return res;
}

ll count_trees(vector<vector<int>>& ed, int root) {
	return rec(ed, 1 << root, 0);
}

int main() {
	for (int it = 0; it < (100); ++it) {
		int n = rand() % 7 + 1;
		int m = rand() % 10;
		int root = rand() % n;
		vector<vector<int>> ed(n);
		vector<vector<ll>> mat(n, vector<ll>(n));
		for (int i = 0; i < (m); ++i) {
			int a = rand() % n;
			int b = rand() % n;
			ed[a].push_back(b);
			mat[b][b]++;
			mat[a][b]--;
		}
		ll num_trees = count_trees(ed, root);

		mat.erase(mat.begin() + root);
		for (int i = 0; i < (n-1); ++i) mat[i].erase(mat[i].begin() + root);

		ll num_trees2 = det(mat);
		assert(num_trees == num_trees2);
	}
	cout << "tests passed!" << endl;
}
