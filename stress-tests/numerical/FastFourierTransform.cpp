#include "../utilities/template.h"

#include "../../content/numerical/FastFourierTransform.h"

const double EPS = 1e-8;
int main() {
	int n = 8;
	vector<C> a(n);
	for (int i = 0; i < (n); ++i) a[i] = C(rand() % 10 - 5, rand() % 10 - 5);
	auto aorig = a;
	fft(a);
	for (int k = 0; k < (n); ++k) {
		C sum{};
		for (int x = 0; x < (n); ++x) {
			sum += aorig[x] * polar(1.0, 2 * M_PI * k * x / n);
		}
		assert(norm(sum - a[k]) < 1e-6);
	}

	vector<double> left(4), right(6);
	for(auto &x: left) x = rand() / (RAND_MAX + 1.0) * 10 - 5;
	for(auto &x: right) x = rand() / (RAND_MAX + 1.0) * 10 - 5;
	vector<double> C = conv(left, right);
	for (int i = 0; i < ((int)(left).size() + (int)(right).size() - 1); ++i) {
		double sum = 0;
		for (int j = 0; j < ((int)(left).size()); ++j) if (i - j >= 0 && i - j < (int)(right).size()) {
			sum += left[j] * right[i - j];
		}
		assert(abs(sum - C[i]) < EPS);
	}
	cout<<"tests passed!"<<endl;
}
