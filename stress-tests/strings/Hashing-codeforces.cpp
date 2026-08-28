#include "../utilities/template.h"

#include "../../content/strings/Hashing-codeforces.h"

int main() {
	BASE = 1'000'003;
	assert((ull)(HashValue(1) * 2 + 1 - 3) == 0);

	for (int it = 0; it < (10000); ++it) {
		int n = rand() % 10;
		int alpha = rand() % 10 + 1;
		string s;
		for (int i = 0; i < (n); ++i) s += (char)('a' + rand() % alpha);
		RollingHash hashes_by_range(s);
		set<string> strs;
		set<ull> hashes;

		for (int left = 1; left <= n; ++left)
			for (int right = left; right <= n; ++right) {
			string sub = s.substr(left - 1, right - left + 1);
			ull hash = (ull) hash_string(sub);
			assert((ull)hashes_by_range.query(left, right) == hash);
			hashes.insert(hash);
			strs.insert(sub);
		}

		// get_hashes
		for (int le = 1; le < (n+1); ++le) {
			auto values = get_hashes(s, le);
			assert((int)values.size() == n - le + 1);
			for (int i = 0; i < n - le + 1; ++i) {
				assert((ull)values[i]
					== (ull)hashes_by_range.query(i + 1, i + le));
			}
		}

		// no collisions
		assert((int)(strs).size() == (int)(hashes).size());
	}
	cout<<"tests passed!"<<endl;
}
