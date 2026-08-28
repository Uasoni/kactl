#include "../UnitTest.h"
#include "../../content/geometry/sphericalDistance.h"

class TestSphericalDistance :
	public UnitTest
{
public:
	TestSphericalDistance() : UnitTest("TestSphericalDistance") { }

	virtual ~TestSphericalDistance() { }

	virtual void run(int subcase) {
		check(spherical_distance(0,0,0,0,5),0.0,"0");
		check(spherical_distance(1,2,3,4,0),0.0,"0");
		check(spherical_distance(0,0,1,0,10),0.0,"0");
		//check(spherical_distance(2,M_PI,1,M_PI,10),0.0,"0");
		check(spherical_distance(M_PI/2,M_PI/2,0,M_PI/2,10),5*M_PI,"0");
	}

	virtual int get_count() const {
		return 1;
	}
};

KACTL_AUTOREGISTER_TEST(TestSphericalDistance);
