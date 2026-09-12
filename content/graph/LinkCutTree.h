/**
 * Author: simon lindholm
 * Date: 2016-07-25
 * Source: https://github.com/ngthanhtrung23/acm_notebook_new/blob/master/data_structure/link_cut_tree.h
 * Description: dynamic connectivity in a forest, vertices $1..n$.
 * link(u,v) requires different components; cut(u,v) requires an existing edge.
 * General graphs need extra logic to find replacement edges after cuts.
 * Each component has a current root (representative):
 * \texttt{access(\&nodes[u])->first()} returns its node pointer.
 * Rerooting/linking/cutting can change it; do not cache representatives.
 * Auxiliary splay trees store paths in root-to-leaf order, not whole components.
 * This version only provides connectivity; access does not isolate a path for
 * aggregation (it may retain nodes below u). Path sums need additional changes.
 * Do not copy the tree or resize nodes: internal pointers must stay valid.
 * Usage: LinkCutTree lc(n);
 *  lc.link(1, 2); // different components
 *  bool same = lc.connected(1, 2);
 *  lc.cut(1, 2); // existing edge
 * Time: all operations take amortized O(\log N).
 * Status: stress-tested a bit for N <= 20
 */
#pragma once

struct LinkCutNode { // p: splay parent; pp: path parent at root
	LinkCutNode *p = 0, *pp = 0, *c[2];
	bool flip = 0;
	LinkCutNode() { c[0] = c[1] = 0; fix(); }
	void fix() {
		if (c[0]) c[0]->p = this;
		if (c[1]) c[1]->p = this;
		// Pull splay subtree aggregates here if augmenting.
	}
	void push_flip() {
		if (!flip) return;
		flip = 0; swap(c[0], c[1]);
		if (c[0]) c[0]->flip ^= 1;
		if (c[1]) c[1]->flip ^= 1;
	}
	int up() { return p ? p->c[1] == this : -1; }
	void rot(int i, int b) {
		int h = i ^ b;
		LinkCutNode *x = c[i], *y = b == 2 ? x : x->c[h], *z = b ? y : x;
		if ((y->p = p)) p->c[up()] = y;
		c[i] = z->c[i ^ 1];
		if (b < 2) {
			x->c[h] = y->c[h ^ 1];
			y->c[h ^ 1] = x;
		}
		z->c[i ^ 1] = this;
		fix(); x->fix(); y->fix();
		if (p) p->fix();
		swap(pp, y->pp);
	}
	void splay() { // Splay to top; push flip.
		for (push_flip(); p; ) {
			if (p->p) p->p->push_flip();
			p->push_flip(); push_flip();
			int c1 = up(), c2 = p->up();
			if (c2 == -1) p->rot(c1, 2);
			else p->p->rot(c2, c1 != c2);
		}
	}
	LinkCutNode* first() { // Splay leftmost path node to top.
		push_flip();
		return c[0] ? c[0]->first() : (splay(), this);
	}
};

struct LinkCutTree {
	vector<LinkCutNode> nodes;
	LinkCutTree(int n) : nodes(n + 1) {}

	void link(int u, int v) { // add an edge (u, v)
		assert(!connected(u, v));
		make_root(&nodes[u]);
		nodes[u].pp = &nodes[v];
	}
	void cut(int u, int v) { // remove an edge (u, v)
		LinkCutNode *x = &nodes[u], *top = &nodes[v];
		make_root(top); x->splay();
		assert(top == (x->pp ?: x->c[0]));
		if (x->pp) x->pp = 0;
		else {
			x->c[0] = top->p = 0;
			x->fix();
		}
	}
	bool connected(int u, int v) { // are u, v in the same tree?
		LinkCutNode* nu = access(&nodes[u])->first();
		return nu == access(&nodes[v])->first();
	}
	// Make u the component root; keep edges unchanged.
	void make_root(LinkCutNode* u) {
		access(u);
		u->splay();
		if(u->c[0]) { // Reverse the path of old ancestors.
			u->c[0]->p = 0;
			u->c[0]->flip ^= 1;
			u->c[0]->pp = u;
			u->c[0] = 0;
			u->fix();
		}
	}
	// Join paths to component root; return the splay root.
	// result->first() gives the component representative.
	LinkCutNode* access(LinkCutNode* u) {
		u->splay();
		while (LinkCutNode* pp = u->pp) {
			pp->splay(); u->pp = 0;
			if (pp->c[1]) { // Keep the detached path's parent.
				pp->c[1]->p = 0; pp->c[1]->pp = pp; }
			pp->c[1] = u; pp->fix(); u = pp;
		}
		return u;
	}
};
