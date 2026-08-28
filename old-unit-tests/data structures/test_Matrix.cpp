#include "../UnitTest.h"
#include "../../content/data-structures/Matrix.h"

class TestMatrix :
	public UnitTest
{
public:
	ifstream in;
	int cases;

	TestMatrix() : UnitTest("TestMatrix") {
		in.open("matrix.in");
		in >> cases;
	}

	virtual ~TestMatrix() {
		in.close();
	}

	virtual void run(int subcase) {
		if (subcase == 0) {
			matrix<int> A(3,3,2);
			check((A+A)(2,2),4);
			check(A*A,matrix<int>(3,3,12));
			A(1,2) = 3;
			//cout << A;
			check(A^1,A,"A^1=A");
			check(A^2,A*A,"A^2=A*A");
 			matrix<int> b(3,1,3);
 			A(1,2) = 5;
 			b(2,0) = 0;
 			b/2 + A*A*b*3 + (A^3)*b - A->*A*b + 5;
			return;
		}
		matrix<int> A,B,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10;
		int c;
		in >> A >> B >> c;
		in >> r1 >> r2 >> r3 >> r4 >> r5 >> r6 >> r7 >> r8 >> r9 >> r10; 
		check(A+B,r1,"plus");
		check(A-B,r2,"minus");
		check(A->*B,r3,"element-wise times");
		check(A/B,r4,"divide");
		check(A*B,r5,"times");
		check(A+c,r6,"plus scalar");
		check(A-c,r7,"minus scalar");
		check(A*c,r8,"times scalar");
		check(A/c,r9,"divide by scalar");
		check(A^c,r10,"power");
	}

	virtual int get_count() const {
		return cases+1;
	}
};

KACTL_AUTOREGISTER_TEST(TestMatrix);
