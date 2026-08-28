#include "../UnitTest.h"
#include "../../content/numerical/goldenSectionSearch.h"

double func1(double x) {
	return 5+(x-3)*(x-3);
}

class TestGoldenSectionSearch :
	public UnitTest
{
public:
	TestGoldenSectionSearch() : UnitTest("TestGoldenSectionSearch") { }

	virtual ~TestGoldenSectionSearch() { }

	virtual void run(int subcase) {
		double e = 1e-6;
		if (subcase == 0) {
			double res = gss(-100,100,func1,e);
			if (abs(res-3) > e)
				fail("");
		} else if (subcase == 1) {
			double res = gss(5,100,func1,e);
			if (abs(res-5) > e)
				fail("");
		} else if (subcase == 2) {
			double res = gss(-10000,2,func1,e);
			if (abs(res-2) > e)
				fail("");
		} else if (subcase == 3) {
			double res = gss(-4,1,sin,e);
			if (abs(res+M_PI/2) > e)
				fail("");
		} else if (subcase == 4) {
			double res = gss(0,1,sin,e);
			if (abs(res-0) > e)
				fail("");
		} else if (subcase == 5) {
			double res = gss(0,6,cos,e);
			if (abs(res-M_PI) > e)
				fail("");
		}
	}

	virtual int get_count() const {
		return 6;
	}
};

KACTL_AUTOREGISTER_TEST(TestGoldenSectionSearch);
