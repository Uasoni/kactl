#include "../UnitTest.h"
#include "../../content/geometry/circleTangents.h"

class TestCircleTangents :
	public UnitTest
{
public:
	TestCircleTangents() : UnitTest("TestCircleTangents") { }

	virtual ~TestCircleTangents() { }

	virtual void run(int subcase) {
		typedef Point<double> P;
		pair<P,P> p = circle_tangents(P(100,1),P(0,0),1);
		cout << p.first << p.second << endl;
		p = circle_tangents(P(104,106),P(100,100),1);
		cout << p.first << p.second << endl;
		p = circle_tangents(P(104,106),P(100,100),2);
		cout << p.first << p.second << endl;
		p = circle_tangents(P(100,100),P(104,106),1);
		cout << p.first << p.second << endl;
		p = circle_tangents(P(0,0),P(1,0),2);
		cout << p.first << p.second << endl;
	}

	virtual int get_count() const {
		return 1;
	}
};

KACTL_AUTOREGISTER_TEST(TestCircleTangents);
