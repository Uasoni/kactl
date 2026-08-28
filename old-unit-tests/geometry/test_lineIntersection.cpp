#include "../UnitTest.h"
#include "../../content/geometry/lineIntersection.h"

class TestLineIntersection :
	public UnitTest
{
public:
	TestLineIntersection() : UnitTest("TestLineIntersection") { }

	virtual ~TestLineIntersection() { }

	virtual void run(int subcase) {
		typedef Point<double> P;
		P r;
		check(line_intersection(P(0,0),P(1,0),P(0,2),P(1,1),r),1);
		if (!(r==P(2,0)))
			fail("1");
		check(line_intersection(P(0,0),P(1,0),P(6,-1),P(0,2),r),1);
		if (!(r==P(4,0)))
			fail("2");
		check(line_intersection(P(1,2),P(3,4),P(-5,1),P(-3,2),r),1);
		if (!(r==P(5,6)))
			fail("3");
		check(line_intersection(P(1,2),P(2,4),P(5,-1),P(1,-9),r),0);
		check(line_intersection(P(1,2),P(2,4),P(9,18),P(7,14),r),-1);
	}

	virtual int get_count() const {
		return 1;
	}
};

KACTL_AUTOREGISTER_TEST(TestLineIntersection);
