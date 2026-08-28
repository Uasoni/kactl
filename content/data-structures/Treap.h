/**
 * Author: someone on codeforces
 * Date: 2017-03-14
 * Source: folklore
 * Description: A short self-balancing tree. it acts as a
 *  sequential container with log-time splits/joins, and
 *  is easy to augment with additional data.
 * Time: $O(\log N)$
 * Status: stress-tested
 */
#pragma once

struct TreapNode {
	TreapNode *l = 0, *r = 0;
	int val, y, c = 1;
	TreapNode(int val) : val(val), y(rand()) {}
	void recalc();
};

int cnt(TreapNode* n) { return n ? n->c : 0; }
void TreapNode::recalc() { c = cnt(l) + cnt(r) + 1; }

template<class F> void each(TreapNode* n, F f) {
	if (n) { each(n->l, f); f(n->val); each(n->r, f); }
}

pair<TreapNode*, TreapNode*> split(TreapNode* n, int k) {
	if (!n) return {};
	if (cnt(n->l) >= k) { // "n->val >= k" for lower_bound(k)
		auto [left, right] = split(n->l, k);
		n->l = right;
		n->recalc();
		return {left, n};
	} else {
		auto [left, right] = split(n->r,k - cnt(n->l) - 1); // and just "k"
		n->r = left;
		n->recalc();
		return {n, right};
	}
}

TreapNode* merge(TreapNode* l, TreapNode* r) {
	if (!l) return r;
	if (!r) return l;
	if (l->y > r->y) {
		l->r = merge(l->r, r);
		return l->recalc(), l;
	} else {
		r->l = merge(l, r->l);
		return r->recalc(), r;
	}
}

TreapNode* insert_at_internal(TreapNode* t, TreapNode* n, int pos) {
	auto [l,r] = split(t, pos);
	return merge(merge(l, n), r);
}

TreapNode* insert_at(TreapNode* tree, TreapNode* node, int position) {
	return insert_at_internal(tree, node, position - 1);
}

// move the 1-indexed inclusive range [left, right] to position target
void move(TreapNode*& t, int left, int right, int target) {
	int l = left - 1, r = right, k = target - 1;
	TreapNode *a, *b, *c;
	tie(a,b) = split(t, l); tie(b,c) = split(b, r - l);
	if (k <= l) t = merge(insert_at_internal(a, b, k), c);
	else t = merge(a, insert_at_internal(c, b, k - r));
}
