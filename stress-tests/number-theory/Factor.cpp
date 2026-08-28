#include "../utilities/template.h"

#include "../../content/number-theory/Factor.h"

mt19937_64 uni(time(0));
void assert_valid(ull n, vector<ull> pr_fac){
	ull cur=1;
	for (auto i: pr_fac){
		if (!is_prime(i)){
			cout<<n<<endl;
			cout<<i<<endl;
			assert(is_prime(i));
		}
		cur *= i;
	}
	if (cur!= n)
		cout<<cur<<' '<<n<<endl;
	assert(cur == n);
}
int main() {
	assert(factor(1).empty());
	assert(factor(2) == vector<ull>{2});
	assert((factor(2299) == vector<ull>{11, 19, 11}));
	for (int n = 2; n < (1e5); ++n) {
		auto res = factor(n);
		assert_valid(n, res);
		res = factor(n*ll(n));
		assert_valid(n*ll(n), res);
	}
	for (int i = 2; i < (1e5); ++i) {
		ull n = 1 + (uni()%(3ul<<61));
		auto res = factor(n);
		assert_valid(n, res);
	}
	for (int i = 0; i < (1e5); ++i) {
		// max number that modmul can handle
		ull n = 7268172458553106874 - i;
		auto res = factor(n);
		assert_valid(n, res);
	}
	cout<<"tests passed!"<<endl;
}
