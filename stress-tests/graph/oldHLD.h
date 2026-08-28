
#include "../../content/data-structures/SegmentTree.h"

struct HldNode {
	int d, par, val, chain_id = -1, pos = -1;
};

struct HldChain {
	int par, val;
	vector<int> nodes;
	SegmentTree tree;
};

struct HeavyLightDecomposition {
	typedef int T;
	const T LOW = -(1<<29);
	void f(T& a, T b) { a = max(a, b); }

	vector<HldNode> nodes;
	vector<HldChain> chains;

	HeavyLightDecomposition(vector<vector<pii>>& g) : nodes((int)(g).size()) {
		dfs(1, -1, g, 0);
		for(auto &c: chains) {
			c.tree = {(int)(c.nodes).size(), 0};
			for (int ni : c.nodes)
				c.tree.update(nodes[ni].pos + 1, nodes[ni].val);
		}
	}

	void update(int node_index, T val) {
		HldNode& n = nodes[node_index]; n.val = val;
		if (n.chain_id != -1) chains[n.chain_id].tree.update(n.pos + 1, val);
	}

	int pard(HldNode& nod) {
		if (nod.par == -1) return -1;
		return nodes[nod.chain_id == -1 ? nod.par : chains[nod.chain_id].par].d;
	}

	// query all *edges* between n1, n2
	pair<T, int> query(int i1, int i2) {
		T ans = LOW;
		while(i1 != i2) {
			HldNode n1 = nodes[i1], n2 = nodes[i2];
			if (n1.chain_id != -1 && n1.chain_id == n2.chain_id) {
				int lo = n1.pos, hi = n2.pos;
				if (lo > hi) swap(lo, hi);
				f(ans, chains[n1.chain_id].tree.query(lo + 1, hi));
				i1 = i2 = chains[n1.chain_id].nodes[hi];
			} else {
				if (pard(n1) < pard(n2))
					n1 = n2, swap(i1, i2);
				if (n1.chain_id == -1)
					f(ans, n1.val), i1 = n1.par;
				else {
					HldChain& c = chains[n1.chain_id];
					f(ans, n1.pos ? c.tree.query(n1.pos + 1, (int)(c.nodes).size())
					              : c.tree.s[1]);
					i1 = c.par;
				}
			}
		}
		return make_pair(ans, i1);
	}

	// query all *nodes* between n1, n2
	pair<T, int> query2(int i1, int i2) {
		pair<T, int> ans = query(i1, i2);
		f(ans.first, nodes[ans.second].val);
		return ans;
	}

	pii dfs(int at, int par, vector<vector<pii>>& g, int d) {
		nodes[at].d = d; nodes[at].par = par;
		int sum = 1, ch, nod, sz;
		tuple<int,int,int> mx(-1,-1,-1);
		for(auto &e: g[at]){
			if (e.first == par) continue;
			tie(sz, ch) = dfs(e.first, at, g, d+1);
			nodes[e.first].val = e.second;
			sum += sz;
			mx = max(mx, make_tuple(sz, e.first, ch));
		}
		tie(sz, nod, ch) = mx;
		if (2*sz < sum) return pii(sum, -1);
		if (ch == -1) { ch = (int)chains.size(); chains.emplace_back(); }
		nodes[nod].pos = (int)chains[ch].nodes.size();
		nodes[nod].chain_id = ch;
		chains[ch].par = at;
		chains[ch].nodes.push_back(nod);
		return pii(sum, ch);
	}
};
