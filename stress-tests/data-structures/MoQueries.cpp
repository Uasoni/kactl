#include "../utilities/template.h"

// #include "../../content/data-structures/mo_queries.h"

int cur_l = 0, cur_r = 0, ops = 0;
void add(int ind, int end) {
	if (cur_l != cur_r) {
		if (end == 0) assert(ind == cur_l - 1);
		else assert(ind == cur_r);
	}
	if (cur_l == cur_r) cur_l = ind, cur_r = ind + 1;
	else if (ind == cur_r) cur_r++;
	else cur_l--;
	ops++;
}
void del(int ind, int end) {
	if (end == 0) assert(ind == cur_l);
	else assert(ind == cur_r - 1);
	if (ind == cur_r - 1) cur_r--;
	else cur_l++;
	assert(cur_l <= cur_r);
	ops++;
}

int calc() {
	return cur_l == cur_r ? -1 : cur_l + (cur_r - cur_l) * 10;
}

int blk; // ~N/sqrt(Q)
vector<int> mo(vector<pii> Q) {
	int L = 0, R = 0;
	vector<int> s((int)(Q).size()), res = s;
	auto order_key = [&](pii query) {
		return pii(query.first / blk, query.second ^ -(query.first / blk & 1));
	};
	iota(begin(s), end(s), 0);
	sort(begin(s), end(s), [&](int s, int t) { return order_key(Q[s]) < order_key(Q[t]); });
	for (int qi : s) {
		pii q = Q[qi];
		while (L > q.first) add(--L, 0);
		while (R < q.second) add(R++, 1);
		while (L < q.first) del(L++, 0);
		while (R > q.second) del(--R, 1);
		res[qi] = calc();
	}
	return res;
}

void test(int n, int q) {
	cur_l = cur_r = ops = 0;
	blk = max((int)(n / sqrt(max(q, 1))), 1);
	vector<pii> queries(q);
	for (auto& pa : queries) {
		pa.first = rand() % n;
		pa.second = rand() % n;
		if (pa.first > pa.second)
			swap(pa.first, pa.second);
	}
	vector<int> res = mo(queries);
	for (int i = 0; i < (q); ++i) {
		int l = queries[i].first, r = queries[i].second;
		if (l == r) {
			assert(res[i] == -1);
		} else {
			assert(res[i] == l + (r - l) * 10);
		}
	}
	// (this inequality holds for random queries; in general it's off by a small constant)
	if (n > 100 && q > 100) {
		// cout << n << ' ' << q << ' ' << ops / (n * sqrt(q)) << endl;
		assert(ops < n * sqrt(q));
	}
}

namespace mo_tree {

vector<int> vals;
int sum;
deque<int> path;
void add(int i, int end) {
	sum += vals[i];
	ops++;
	if (end == 0) path.push_front(i);
	else path.push_back(i);
}
void del(int i, int end) {
	sum -= vals[i];
	ops++;
	assert(!path.empty());
	if (end == 0) {
		assert(path.front() == i);
		path.pop_front();
	} else {
		assert(path.back() == i);
		path.pop_back();
	}
}
int calc() { return sum; }

vector<int> mo_tree(vector<array<int, 2>> Q, vector<vector<int>>& ed, int root=0){
	int N = (int)(ed).size(), pos[2] = {};
	vector<int> s((int)(Q).size()), res = s, I(N), L(N), R(N), in(N), par(N);
	add(0, 0), in[0] = 1;
	auto dfs = [&](int x, int p, int dep, auto& f) -> void {
		par[x] = p;
		L[x] = N;
		if (dep) I[x] = N++;
		for (int y : ed[x]) if (y != p) f(y, x, !dep, f);
		if (!dep) I[x] = N++;
		R[x] = N;
	};
	dfs(root, -1, 0, dfs);
	auto order_key = [&](const array<int, 2>& query) {
		return pii(I[query[0]] / blk,
			I[query[1]] ^ -(I[query[0]] / blk & 1));
	};
	iota(begin(s), end(s), 0);
	sort(begin(s), end(s), [&](int s, int t) { return order_key(Q[s]) < order_key(Q[t]); });
	for (int qi : s) for (int end = 0; end < (2); ++end) {
		int &a = pos[end], b = Q[qi][end], i = 0;
		auto step = [&](int next) {
			if (in[next]) del(a, end), in[a] = 0;
			else add(next, end), in[next] = 1;
			a = next;
		};
		while (!(L[b] <= L[a] && R[a] <= R[b]))
			I[i++] = b, b = par[b];
		while (a != b) step(par[a]);
		while (i--) step(I[i]);
		if (end) res[qi] = calc();
	}
	return res;
}

}

void test_tr(int n, int q) {
	ops = 0;
	blk = max((int)(n / sqrt(max(q, 1))), 1);
	vector<array<int, 2>> queries(q);
	for (auto& pa : queries) {
		pa[0] = rand() % n;
		pa[1] = rand() % n;
	}
	vector<int> par(n), val(n);
	for (int i = 1; i < (n); ++i) par[i] = rand() % i;
	for (int i = 0; i < (n); ++i) val[i] = rand() % 1000;
	vector<vector<int>> ed(n);
	for (int i = 1; i < (n); ++i) ed[par[i]].push_back(i), ed[i].push_back(par[i]);
	mo_tree::vals = val;
	mo_tree::sum = 0;
	mo_tree::path.clear();
	vector<int> res = mo_tree::mo_tree(queries, ed);
	vector<int> seen(n);
	for (int i = 0; i < (q); ++i) {
		// tree depth is logarithmic, so compute query answers naively
		int l = queries[i][0], r = queries[i][1];
		int at = l;
		while (at != 0) seen[at] = 1, at = par[at];
		seen[at] = 1;
		int sum = 0;
		while (!seen[r]) sum += val[r], r = par[r];
		at = l;
		while (at != 0) seen[at] = 0, at = par[at];
		seen[at] = 0;
		while (l != r) sum += val[l], l = par[l];
		sum += val[l];
		assert(res[i] == sum);
	}
}

int main() {
	srand(2);
	for (int it = 0; it < (10); ++it) for (int n = 1; n < (15); ++n) for (int q = 0; q < (n*n); ++q) {
		test_tr(n, q);
	}
	test_tr(100'000, 100'000);
	test_tr(1000, 100'000);
	test_tr(100'000, 1000);
	test(100'000, 100'000);
	test(1000, 100'000);
	test(100'000, 1000);
	for (int it = 0; it < (10); ++it) for (int n = 1; n < (15); ++n) for (int q = 0; q < (n*n); ++q) {
		test(n, q);
	}
	cout << "tests passed!" << endl;
}
