#include "../UnitTest.h"
#include "../../content/geometry/convexHull.h"
typedef Point<double> P;

template<class T>
ostream & operator<<(ostream & os, const vector<T> p) {
	os << "[ ";
	for(auto &it:p) os << it << " ";
	os << "]";
	return os;
}

class TestConvexHull : public UnitTest {
public:
	ifstream in;
	int cases;
	TestConvexHull() : UnitTest("TestConvexHull") {
		ifstream file("convex_hull.data");
		int N = 0, n;
		while (file >> n) {
			double d;
			for (int i = 0; i < (n); ++i) file >> d >> d;
			++N;
		}
		cases = N/2;
		file.close();

		in.open("convex_hull.data");
	}

	virtual ~TestConvexHull() {
		in.close();
	}

	vector<P> read_polygon() {
		int n;
		in >> n;
		vector<P> p(n);
		for (int i = 0; i < (n); ++i) in >> p[i];
		return p;
	}


	virtual void run(int subcase) {
		vector<P> p = read_polygon(), wanted = read_polygon();
		vector<P> res(p.begin(),convex_hull(p.begin(),p.end()));

		sort(res.begin(),res.end());
		sort(wanted.begin(),wanted.end());
		stringstream ss;
		ss << p << endl << " -> " << res << endl << "!=" << wanted << endl;
		string s = ss.str();
		check(res.size(),wanted.size(),s);
		for (int i = 0; i < (res.size()); ++i)
			if (!(res[i] == wanted[i]))
				fail(s);
	}

	void test(int subcase) {
	    run(subcase);
		typedef Point<double> P;
		P p1[3] = {P(1,1),P(3,2),P(1,5)};
		check(convex_hull(p1,p1+3),p1+3);

		P p2[] = {P(0,0),P(1,0),P(2,0),P(2,1),P(2,2),P(1,2),P(0,2),P(0,1)};
		int n = convex_hull(p2,p2+8)-p2;
		cout << endl << n << " ";
		for (int i = 0; i < (n); ++i) cout << p2[i] << " ";
		cout << endl;

		P p3[] = {P(0,0),P(1,0),P(2,0),P(2,1),P(2,2),P(1,2),P(0,2),P(1,1)};
		n = convex_hull(p3,p3+8)-p3;
		cout << endl << n << " ";
		for (int i = 0; i < (n); ++i) cout << p2[i] << " ";
		cout << endl;
	}

	virtual int get_count() const {
		return cases;
	}
};

KACTL_AUTOREGISTER_TEST(TestConvexHull);
