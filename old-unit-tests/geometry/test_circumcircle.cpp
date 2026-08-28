#include "../UnitTest.h"
#include "../../content/geometry/circumcircle.h"

class TestCircumcircle :
	public UnitTest
{
public:
	typedef Point<double> P;
	TestCircumcircle() : UnitTest("TestCircumcircle") { }

	virtual ~TestCircumcircle() { }

	void test(P a, P b, P c_point, double r, P c) {
		stringstream ss;
		double rp = cc_radius(a,b,c_point);
		P cp = cc_center(a,b,c_point);
		ss << a << b << c_point << " gives radius " << rp << " and center " << cp
				<< ". expected radius " << r << " and center " << c;
		//cout << ss.str() << endl;
		if (abs(r-rp) > 1e-10 || (c-cp).dist() > 1e-10)
			fail(ss.str());
	}
	void test1(P a, P b, P c_point, double r, P c) {
		test(a,b,c_point,r,c);
		test(a,c_point,b,r,c);
		test(b,a,c_point,r,c);
		test(b,c_point,a,r,c);
		test(c_point,a,b,r,c);
		test(c_point,b,a,r,c);
	}

	virtual void run(int subcase) {
		test1(P(2,1),P(1,2),P(3,2), 1, P(2,2));
		test1(P(5,1),P(2,2),P(1,3), 5, P(5,6));
		test1(P(0,6),P(2,2),P(1,3), 5, P(5,6));
		test1(P(0,6),P(5,1),P(1,3), 5, P(5,6));
		test1(P(0,0),P(0,1),P(1,0), 1/sqrt(2), P(.5,.5));
		test1(P(0,0),P(0,1),P(0,2), 1/.0, P(.0/.0,.0/.0));
	}

	virtual int get_count() const {
		return 1;
	}
};

KACTL_AUTOREGISTER_TEST(TestCircumcircle);
