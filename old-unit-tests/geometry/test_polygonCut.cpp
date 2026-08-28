#include "../UnitTest.h"
#include "../../content/geometry/polygonCut.h"

typedef Point<int> P;
class TestPolygonCut :
	public UnitTest
{
public:
	TestPolygonCut() : UnitTest("TestPolygonCut") { }

	virtual ~TestPolygonCut() { }

	string str(vector<P> p, int n) {
		stringstream ss;
		ss << n << " : ";
		for(auto &i:p) ss << i;
		return ss.str();
	}


	virtual void run(int subcase) {
		const int N = 8;
		P p[N] = {P(-1,0),P(1,2),P(1,0),P(2,0),P(0,-1),P(3,-1),P(0,-3),P(0,0)};
		//P p[] = {P(0,0),P(1,0),P(1,1)};

		if (subcase == 0) {
			vector<P> res = polygon_cut(p,p+N,P(100,1),P(100,0));
			P w[8] = {P(-1,0),P(1,2),P(1,0),P(2,0),P(0,-1),P(3,-1),P(0,-3),P(0,0)};
			string s = str(res,subcase);
			check((int)res.size(),8,s);
			for (int i = 0; i < (res.size()); ++i)
				check(res[i],w[i],s);
		} else if (subcase == 1) {
			vector<P> res = polygon_cut(p,p+N,P(100,1),P(100,2));
			P w[0] = {};
			string s = str(res,subcase);
			check((int)res.size(),0,s);
			for (int i = 0; i < (res.size()); ++i)
				check(res[i],w[i],s);
		} else if (subcase == 2) {
			vector<P> res = polygon_cut(p,p+N,P(0,0),P(1,0));
			P w[5] = {P(2,0),P(0,-1),P(3,-1),P(0,-3),P(0,0)};
			string s = str(res,subcase);
			check((int)res.size(),5,s);
			for (int i = 0; i < (res.size()); ++i)
				check(res[i],w[i],s);
		} else if (subcase == 3) {
			vector<P> res = polygon_cut(p,p+N,P(2,0),P(1,0));
			P w[3] = {P(-1,0),P(1,2),P(1,0)};
			string s = str(res,subcase);
			check((int)res.size(),3,s);
			for (int i = 0; i < (res.size()); ++i)
				check(res[i],w[i],s);
		} else if (subcase == 4) {
			vector<P> res = polygon_cut(p,p+N,P(2,1),P(1,1));
			P w[3] = {P(0,1),P(1,2),P(1,1)};
			string s = str(res,subcase);
			check((int)res.size(),3,s);
			for (int i = 0; i < (res.size()); ++i)
				check(res[i],w[i],s);
		}
	}

	virtual int get_count() const {
		return 5;
	}
};

KACTL_AUTOREGISTER_TEST(TestPolygonCut);
