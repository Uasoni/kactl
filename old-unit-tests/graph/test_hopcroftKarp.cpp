#include "../UnitTest.h"
#include "../../content/graph/HopcroftKarp.h"

class TestHopcorftKarp :
	public UnitTest
{
	public:
		ifstream in;
		int cases;

		TestHopcorftKarp() : UnitTest("TestHopcorftKarp") {
			in.open("matching.in");
			in >> cases;
		}

		virtual ~TestHopcorftKarp() {
			in.close();
		}

		virtual void run(int subcase) {
			int n, m, k;
			in >> n >> m >> k;
			vector<vector<int> > g(n);
			for (int i = 0; i < k; i++) {
				int x, y; in >> x >> y;
				x--; y--;
				g[x].push_back(y);
			}
			vector<int> b(m, -1);
			int res = hopcroft_karp(g, b);
			if (subcase == 0) check(res, 2, "0");
			if (subcase == 1) check(res, 43, "1");
			if (subcase == 2) check(res, 40, "2");
			if (subcase == 3) check(res, 40, "3");
			if (subcase == 4) check(res, 47, "4");
		}

		virtual int get_count() const {
			return cases;
		}
};

KACTL_AUTOREGISTER_TEST(TestHopcorftKarp);
