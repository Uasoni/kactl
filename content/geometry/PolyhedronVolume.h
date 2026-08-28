/**
 * Author: mattias de zalenski
 * Date: 2002-11-04
 * Description: magic formula for the volume of a polyhedron. faces should Point outwards.
 * Status: tested
 */
#pragma once

template<class V, class L>
double signed_poly_volume(const V& p, const L& trilist) {
	double v = 0;
	for (auto i : trilist) v += p[i.a].cross(p[i.b]).dot(p[i.c]);
	return v / 6;
}
