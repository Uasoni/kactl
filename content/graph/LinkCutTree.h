/**
 * Author: simon lindholm
 * Date: 2016-07-25
 * Source: https://github.com/ngthanhtrung23/acm_notebook_new/blob/master/data_structure/link_cut_tree.h
 * Description: represents a forest of unrooted trees. you can add and remove
 * edges (as long as the result is still a forest), and check whether
 * two nodes are in the same tree.
 * vertices are numbered $1..n$.
 * Time: all operations take amortized O(\log N).
 * Status: stress-tested a bit for N <= 20
 */
#pragma once

struct LinkCutNode { // splay tree. root's pp contains tree's parent.
	LinkCutNode *p = 0, *pp = 0, *c[2];
	bool flip = 0;
	LinkCutNode() { c[0] = c[1] = 0; fix(); }
	void fix() {
		if (c[0]) c[0]->p = this;
		if (c[1]) c[1]->p = this;
		// (+ update sum of subtree elements etc. if wanted)
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
	void splay() { /// splay this up to the root. always finishes without flip set.
		for (push_flip(); p; ) {
			if (p->p) p->p->push_flip();
			p->push_flip(); push_flip();
			int c1 = up(), c2 = p->up();
			if (c2 == -1) p->rot(c1, 2);
			else p->p->rot(c2, c1 != c2);
		}
	}
	LinkCutNode* first() { /// return the min element of the subtree rooted at this, splayed to the top.
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
	void make_root(LinkCutNode* u) { /// move u to root of represented tree.
		access(u);
		u->splay();
		if(u->c[0]) {
			u->c[0]->p = 0;
			u->c[0]->flip ^= 1;
			u->c[0]->pp = u;
			u->c[0] = 0;
			u->fix();
		}
	}
	LinkCutNode* access(LinkCutNode* u) { /// move u to root aux tree. return the root of the root aux tree.
		u->splay();
		while (LinkCutNode* pp = u->pp) {
			pp->splay(); u->pp = 0;
			if (pp->c[1]) {
				pp->c[1]->p = 0; pp->c[1]->pp = pp; }
			pp->c[1] = u; pp->fix(); u = pp;
		}
		return u;
	}
};
