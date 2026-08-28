#include "../utilities/template.h"

#include "../../content/geometry/ClosestPair.h"
typedef Point<ll> point_type;

namespace old {
template<class it>
bool it_less(const it& i, const it& j) { return *i < *j; }
template<class it>
bool y_it_less(const it& i,const it& j) {return i->y < j->y;}

template<class it, class i_it> /* i_it = vector<it>::iterator */
double cp_sub(i_it ya, i_it yaend, i_it xa, it &i1, it &i2) {
	typedef typename iterator_traits<it>::value_type point_type;
	int n = yaend-ya, split = n/2;
	if(n <= 3) { // base case
		double a = (*xa[1]-*xa[0]).dist(), b = 1e50, c = 1e50;
		if(n==3) b=(*xa[2]-*xa[0]).dist(), c=(*xa[2]-*xa[1]).dist();
		if(a <= b) { i1 = xa[1];
			if(a <= c) return i2 = xa[0], a;
			else return i2 = xa[2], c;
		} else { i1 = xa[2];
			if(b <= c) return i2 = xa[0], b;
			else return i2 = xa[1], c;
	}	}
	vector<it> ly, ry, stripy;
	point_type splitp = *xa[split];
	double splitx = splitp.x;
	for(i_it i = ya; i != yaend; ++i) { // divide
		if(*i != xa[split] && (**i-splitp).dist2() < 1e-12)
			return i1 = *i, i2 = xa[split], 0;// nasty special case!
		if (**i < splitp) ly.push_back(*i);
		else ry.push_back(*i);
	} // assert((signed)lefty.size() == split)
	it j1, j2; // conquer
	double a = cp_sub(ly.begin(), ly.end(), xa, i1, i2);
	double b = cp_sub(ry.begin(), ry.end(), xa+split, j1, j2);
	if(b < a) a = b, i1 = j1, i2 = j2;
	double a2 = a*a;
	for(i_it i = ya; i != yaend; ++i) { // create strip (y-sorted)
		double x = (*i)->x;
		if(x >= splitx-a && x <= splitx+a) stripy.push_back(*i);
	}
	for(i_it i = stripy.begin(); i != stripy.end(); ++i) {
		const point_type &p1 = **i;
		for(i_it j = i+1; j != stripy.end(); ++j) {
			const point_type &p2 = **j;
			if(p2.y-p1.y > a) break;
			double d2 = (p2-p1).dist2();
			if(d2 < a2) i1 = *i, i2 = *j, a2 = d2;
	}	}
	return sqrt(a2);
}

template<class it> // it is random access iterators of Point<T>
double closestpair(it begin, it end, it &i1, it &i2 ) {
	vector<it> xa, ya;
	assert(end-begin >= 2);
	for (it i = begin; i != end; ++i)
		xa.push_back(i), ya.push_back(i);
	sort(xa.begin(), xa.end(), it_less<it>);
	sort(ya.begin(), ya.end(), y_it_less<it>);
	return cp_sub(ya.begin(), ya.end(), xa.begin(), i1, i2);
}
}

int main() {
	// compare against the old code
	ll sum = 0;
	int mode = 1;
	if (mode != 0) for (int it = 0; it < (100); ++it) {
		// clog << it << ' ';
		int n = 100000;
		int maxx = rand() % 1000000 + 1;
		int maxy = rand() % 1000000 + 1;
		int biasx = -100;
		int biasy = -100;
		vector<point_type> ps;
		for (int i = 0; i < (n); ++i) {
			int x = rand() % maxx + biasx;
			int y = rand() % maxy + biasy;
			ps.emplace_back(x, y);
		}
		ll found_dist = -1, old_dist = -1, the_dist = -1;
		if (mode == 1 || mode == 3) {
			auto pa = closest(ps);
			the_dist = found_dist = (pa.first - pa.second).dist2();
		}
		if (mode == 2 || mode == 3) {
			vector<point_type>::iterator i1, i2;
			old::closestpair(begin(ps), end(ps), i1, i2);
			the_dist = old_dist = (*i1 - *i2).dist2();
		}
		sum += the_dist;
		// cerr << the_dist << endl;
		if (mode == 3 && old_dist != found_dist) {
			cerr << "failed at " << it << endl;
			return 1;
		}
	}
	// cout << sum << endl;

	// compare against BruteForce
	for (int it = 0; it < (1'000'000); ++it) {
		int n = rand() % 15 + 2;
		int maxx = rand() % 20 + 1;
		int maxy = rand() % 20 + 1;
		int biasx = rand() % 20 - 10;
		int biasy = rand() % 20 - 10;
		vector<point_type> ps;
		for (int i = 0; i < (n); ++i) {
			int x = rand() % maxx + biasx;
			int y = rand() % maxy + biasy;
			ps.emplace_back(x, y);
		}
		ll min_dist = LLONG_MAX;
		for (int i = 0; i < (n); ++i) for (int j = i+1; j < (n); ++j) {
			min_dist = min(min_dist, (ps[i] - ps[j]).dist2());
		}
		auto pa = closest(ps);
		ll found_dist = (pa.first - pa.second).dist2();
		if (min_dist != found_dist) {
			cerr << "failed at " << it << endl;
			return 1;
		}
	}
	cout<<"tests passed!"<<endl;
}
