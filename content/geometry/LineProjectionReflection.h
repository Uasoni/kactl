/**
 * Author: victor lecomte, chilli
 * Date: 2019-10-29
 * License: CC0
 * Source: https://vlecomte.github.io/cp-geo.pdf
 * Description: projects Point p onto line ab. set refl=true to get reflection
 * of Point p across line ab instead. the wrong Point will be returned if P is
 * an integer Point and the desired Point doesn't have integer coordinates.
 * products of three coordinates are used in intermediate steps so watch out
 * for overflow.
 * Status: stress-tested
 */
#pragma once

#include "Point.h"

template<class P>
P line_proj(P a, P b, P p, bool refl=false) {
	P v = b - a;
	return p - v.perp()*(1+refl)*v.cross(p-a)/v.dist2();
}
