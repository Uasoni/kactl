/**
 * Author: per austrin, ulf lundstrom
 * Date: 2009-04-09
 * License: CC0
 * Source:
 * Description:\\
\begin{minipage}{75mm}
 apply the linear transformation (translation, rotation and scaling) which takes line p0-p1 to line q0-q1 to Point r.
\end{minipage}
\begin{minipage}{15mm}
\vspace{-8mm}
\includegraphics[width=\textwidth]{content/geometry/linearTransformation}
\vspace{-2mm}
\end{minipage}
 * Status: not tested
 */
#pragma once

#include "Point.h"

typedef Point<double> point_type;
point_type linear_transformation(const point_type& p0, const point_type& p1,
		const point_type& q0, const point_type& q1, const point_type& r) {
	point_type dp = p1-p0, dq = q1-q0, num(dp.cross(dq), dp.dot(dq));
	return q0 + point_type((r-p0).cross(num), (r-p0).dot(num))/dp.dist2();
}
