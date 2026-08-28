/**
 * Author: philippe legault
 * Date: 2016
 * License: MIT
 * Source: https://github.com/bathlamos/delaunay-triangulation/
 * Description: fast delaunay triangulation.
 * each circumcircle contains none of the input points.
 * there must be no duplicate points.
 * if all points are on a line, no triangles will be returned.
 * should work for doubles as well, though there may be precision issues in 'circ'.
 * returns triangles in order \{t[0][0], t[0][1], t[0][2], t[1][0], \dots\}, all counter-clockwise.
 * Time: O(n \log n)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"

typedef Point<ll> delaunay_point;
typedef struct QuadEdge* quad_edge;
typedef __int128_t wide_int; // (can be ll if coords are < 2e4)
const delaunay_point ARBITRARY_POINT(LLONG_MAX, LLONG_MAX);

struct QuadEdge {
	quad_edge rot, o; delaunay_point p = ARBITRARY_POINT; bool mark = false;
	delaunay_point& destination() { return r()->p; }
	quad_edge& r() { return rot->rot; }
	quad_edge prev() { return rot->o->rot; }
	quad_edge next() { return r()->prev(); }
} *free_edges;

bool circ(delaunay_point p, delaunay_point a, delaunay_point b,
		delaunay_point c) {
	wide_int p2 = p.dist2(), da = a.dist2() - p2,
		db = b.dist2() - p2, dc = c.dist2() - p2;
	return p.cross(a, b) * dc + p.cross(b, c) * da
		+ p.cross(c, a) * db > 0;
}
quad_edge make_edge(delaunay_point orig, delaunay_point dest) {
	quad_edge r = free_edges ? free_edges : new QuadEdge{new QuadEdge{new QuadEdge{new QuadEdge{0}}}};
	free_edges = r->o; r->r()->r() = r;
	for (int i = 0; i < (4); ++i) r = r->rot, r->p = ARBITRARY_POINT, r->o = i & 1 ? r : r->r();
	r->p = orig; r->destination() = dest;
	return r;
}
void splice(quad_edge a, quad_edge b) {
	swap(a->o->rot->o, b->o->rot->o); swap(a->o, b->o);
}
quad_edge connect(quad_edge a, quad_edge b) {
	quad_edge q = make_edge(a->destination(), b->p);
	splice(q, a->next());
	splice(q->r(), b);
	return q;
}

pair<quad_edge, quad_edge> triangulate_rec(const vector<delaunay_point>& s) {
	if ((int)(s).size() <= 3) {
		quad_edge a = make_edge(s[0], s[1]), b = make_edge(s[1], s.back());
		if ((int)(s).size() == 2) return { a, a->r() };
		splice(a->r(), b);
		auto side = s[0].cross(s[1], s[2]);
		quad_edge c = side ? connect(b, a) : 0;
		return {side < 0 ? c->r() : a, side < 0 ? c : b->r() };
	}

	quad_edge left, right, left_outer, right_outer;
	int half = (int)(s).size() / 2;
	tie(left_outer, left) = triangulate_rec({begin(s), end(s) - half});
	tie(right, right_outer) = triangulate_rec(
		{(int)s.size() - half + begin(s), end(s)});
	while ((right->p.cross(left->destination(), left->p) < 0
			&& (left = left->next()))
		|| (left->p.cross(right->destination(), right->p) > 0
			&& (right = right->r()->o)));
	quad_edge base = connect(right->r(), left);
	if (left->p == left_outer->p) left_outer = base->r();
	if (right->p == right_outer->p) right_outer = base;
	auto valid = [&](quad_edge edge) {
		return edge->destination().cross(base->destination(), base->p) > 0;
	};
	auto discard_edges = [&](quad_edge initial, auto next_edge) {
		quad_edge edge = next_edge(initial);
		if (valid(edge)) while (circ(next_edge(edge)->destination(),
				base->destination(), base->p, edge->destination())) {
			quad_edge next = next_edge(edge);
			splice(edge, edge->prev());
			splice(edge->r(), edge->r()->prev());
			edge->o = free_edges; free_edges = edge; edge = next;
		}
		return edge;
	};
	for (;;) {
		quad_edge left_candidate = discard_edges(base->r(),
			[](quad_edge edge) { return edge->o; });
		quad_edge right_candidate = discard_edges(base,
			[](quad_edge edge) { return edge->prev(); });
		if (!valid(left_candidate) && !valid(right_candidate)) break;
		if (!valid(left_candidate) || (valid(right_candidate)
				&& circ(right_candidate->destination(), right_candidate->p,
					left_candidate->destination(), left_candidate->p)))
			base = connect(right_candidate, base->r());
		else
			base = connect(base->r(), left_candidate->r());
	}
	return {left_outer, right_outer};
}

vector<delaunay_point> triangulate(vector<delaunay_point> pts) {
	sort(begin(pts), end(pts));  assert(unique(begin(pts), end(pts)) == pts.end());
	if ((int)(pts).size() < 2) return {};
	quad_edge e = triangulate_rec(pts).first;
	vector<quad_edge> q = {e};
	int qi = 0;
	while (e->o->destination().cross(e->destination(), e->p) < 0) e = e->o;
	auto add_face = [&](quad_edge edge) {
		quad_edge current = edge;
		do {
			current->mark = true;
			pts.push_back(current->p);
			q.push_back(current->r());
			current = current->next();
		} while (current != edge);
	};
	add_face(e); pts.clear();
	while (qi < (int)q.size())
		if (!(e = q[qi++])->mark) add_face(e);
	return pts;
}
