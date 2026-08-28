#include "../UnitTest.h"
#include "../../content/geometry/sideOf.h"

class TestSideOf :
	public UnitTest
{
public:
	TestSideOf() : UnitTest("TestSideOf") { }

	virtual ~TestSideOf() { }

	virtual void run(int subcase) {
		typedef Point<double> P;
		check(side_of(P(0,0),P(1,0),P(1,1)),1);
		check(side_of(P(0,0),P(1,0),P(1,0)),0);
		check(side_of(P(0,0),P(1,0),P(1,-1)),-1);
		check(side_of(P(1,0),P(0,0),P(1,1)),-1);
		check(side_of(P(1,0),P(0,0),P(1,0)),0);
		check(side_of(P(1,0),P(0,0),P(1,-1)),1);
	}

	virtual int get_count() const {
		return 1;
	}
};

KACTL_AUTOREGISTER_TEST(TestSideOf);
