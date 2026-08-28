#include "../UnitTest.h"
#include "../../content/data-structures/UnionFind.h"

class TestUnionFind :
	public UnitTest
{
public:
	TestUnionFind() : UnitTest("TestUnionFind") { }

	virtual ~TestUnionFind() { }

	virtual void run(int subcase) {
		if (subcase == 0) {
			UnionFind u(5);
			u.join(0, 1);
			if (u.same_set(0, 2))
				fail("0 and 2 should not be in the same set");
			u.join(0, 2);
			if (u.size(2) != 3)
				fail("the size of the set where 2 is should be 3");
			if (!u.same_set(0, 2))
				fail("0 and 2 should be in the same set");
			if (u.same_set(3, 2))
				fail("3 and 2 should not be in the same set");
			if (u.size(3) != 1)
				fail("the size of the set where 3 is should be 1");
			u.join(3, 4);
			if (!u.same_set(3, 4))
				fail("3 and 4 should be in the same set");
			if (u.same_set(4, 2))
				fail("4 and 2 should not be in the same set");
			if (u.size(3) != 2)
				fail("the size of the set where 3 is should be 2");

		}
	}

	virtual int get_count() const {
		return 1;
	}
};

KACTL_AUTOREGISTER_TEST(TestUnionFind);
