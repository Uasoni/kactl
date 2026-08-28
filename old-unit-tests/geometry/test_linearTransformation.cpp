#include "../UnitTest.h"
#include "../../content/geometry/linearTransformation.h"

class TestLinearTransformation :
	public UnitTest
{
public:
	TestLinearTransformation() : UnitTest("TestLinearTransformation") { }

	virtual ~TestLinearTransformation() { }

	virtual void run(int subcase) {
		typedef Point<double> P;
		check(linear_transformation(P(0,0),P(1,0),P(1,1),P(1,2),P(2,1)),P(0,3));
		check(linear_transformation(P(1,1),P(5,4),P(2,-1),P(4,-3),P(8,0)),P(2,-5));
	}

	virtual int get_count() const {
		return 1;
	}
};

KACTL_AUTOREGISTER_TEST(TestLinearTransformation);
