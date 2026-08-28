#include "../UnitTest.h"
#include "../../content/combinatorial/multinomial.h"

class TestMultinomial :
	public UnitTest
{
	public:
		vector<vector<int> > cases;

		TestMultinomial() : UnitTest("TestMultinomial") {
			vector<int> a(2, 2);
			cases.push_back(a);
			a.clear();
			a.push_back(3); a.push_back(4); a.push_back(5);
			cases.push_back(a);
		}

		virtual ~TestMultinomial() {
		}

		virtual void run(int i) {
			int res = multinomial<int, vector<int> >(cases[i].size(), cases[i]);
			if (i == 0)
				check(res, 6, "0");
			if (i == 1)
				check(res, 27720, "0");
		}

		virtual int get_count() const {
			return cases.size();
		}
};

KACTL_AUTOREGISTER_TEST(TestMultinomial);
