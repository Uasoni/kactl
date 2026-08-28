#include "../UnitTest.h"
#include "../../content/various/intervalUnion.h"

class TestIntervalUnion :
	public UnitTest
{
public:
	TestIntervalUnion() : UnitTest("TestIntervalUnion") { }

	virtual ~TestIntervalUnion() { }

	void test(vector<pii> s, vector<pii> r) {
		vector<pii> res = interval_union<int>(s.begin(),s.end());
		stringstream ss;
		trav(i,s) ss << "(" << i.first << "," << i.second << ")";
		ss << endl <<  " -> ";
		trav(i,res) ss << "(" << i.first << "," << i.second << ")";
		ss << endl << " != ";
		trav(i,r) ss << "(" << i.first << "," << i.second << ")";
		if (res != r)
			fail(ss.str());
	}

	virtual void run(int subcase) {
		if (subcase == 0) {
			vector<pii> v;
			v.push_back(pii(1,3));
			v.push_back(pii(2,4));
			vector<pii> r;
			r.push_back(pii(1,4));
			test(v,r);
		} else if (subcase == 1) {
			vector<pii> v;
			v.push_back(pii(4,10));
			v.push_back(pii(1,3));
			v.push_back(pii(11,12));
			v.push_back(pii(2,4));
			vector<pii> r;
			r.push_back(pii(1,10));
			r.push_back(pii(11,12));
			test(v,r);
		}
	}

	virtual int get_count() const {
		return 2;
	}
};

KACTL_AUTOREGISTER_TEST(TestIntervalUnion);
