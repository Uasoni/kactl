#include "../utilities/template.h"

#include "../../content/strings/MinRotation.h"

int min_rotation2(string& v) {
	int n = (int)(v).size();
	string w = v; w.insert(w.end(), begin(v), end(v));
	int j = 0;
	for (int i = 1; i < (n); ++i) {
		if (vector<int>(w.begin() + i, w.begin() + i + n) <
			vector<int>(w.begin() + j, w.begin() + j + n)) j = i;
	}
	return j;
}

void test_perf() {
	string s;
	for (int i = 0; i < (10000000); ++i)
		s += (char)(rand()%400000 < 2);
	cout << min_rotation(s) << endl;
}

int main() {
	for (int it = 0; it < (1000000); ++it) {
		int n = rand() % 10;
		string v;
		for (int i = 0; i < (n); ++i) v += (char)(rand() % 3);
		int r = min_rotation(v);
		int r2 = min_rotation2(v);
		assert(r == r2);
		rotate(v.begin(), v.begin() + r, v.end());
		assert(min_rotation(v) == 0);
		assert(min_rotation2(v) == 0);
	}
	cout<<"tests passed!"<<endl;
}
