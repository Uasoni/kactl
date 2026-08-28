/**
 * Author: ulf lundstrom
 * Date: 2009-04-11
 * License: CC0
 * Source: http://en.wikipedia.org/wiki/circumcircle
 * Description:\\
\begin{minipage}{75mm}
the circumcirle of a triangle is the circle intersecting all three vertices. cc\_radius returns the radius of the circle going through points A, B and C and cc\_center returns the center of the same circle.
\end{minipage}
\begin{minipage}{15mm}
\vspace{-2mm}
\includegraphics[width=\textwidth]{content/geometry/circumcircle}
\end{minipage}
 * Status: tested
 */
#pragma once

#include "Point.h"

typedef Point<double> circle_point;
double cc_radius(const circle_point& A, const circle_point& B, const circle_point& C) {
	return (B-A).dist()*(C-B).dist()*(A-C).dist()/
			abs((B-A).cross(C-A))/2;
}
circle_point cc_center(const circle_point& A, const circle_point& B, const circle_point& C) {
	circle_point b = C-A, c = B-A;
	return A + (b*c.dist2()-c*b.dist2()).perp()/b.cross(c)/2;
}
