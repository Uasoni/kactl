#pragma once

#include "../../content/geometry/Point.h"

template <class T=double>
Point<T> rand_int_pt(int lim) {
	return Point<T>(rand()%(lim*2) - lim, rand()%(lim*2)-lim);
}
