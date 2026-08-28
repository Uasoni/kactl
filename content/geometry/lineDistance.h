/**
 * Author: ulf lundstrom
 * Date: 2009-03-21
 * License: CC0
 * Source: basic math
 * Description:\\
\begin{minipage}{75mm}
returns the signed distance between Point p and the line containing points a and b.
positive value on left side and negative on right as seen from a towards b. a==b gives nan.
P is supposed to be Point<T> or Point3D<T> where T is e.g. double or long long.
it uses products in intermediate steps so watch out for overflow if using int or long long.
using Point3D will always give a non-negative distance. for Point3D, call .dist on the result of the cross product.
\end{minipage}
\begin{minipage}{15mm}
\includegraphics[width=\textwidth]{content/geometry/lineDistance}
\end{minipage}
 * Status: tested
 */
#pragma once

#include "Point.h"

template<class P>
double line_dist(const P& a, const P& b, const P& p) {
	return (double)(b-a).cross(p-a)/(b-a).dist();
}
