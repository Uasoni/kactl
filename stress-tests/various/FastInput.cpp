#include "../utilities/template.h"
#include <unistd.h>

// same as fast_input.h but within a struct, to be able to reset bc/be from outside.
struct InputReader {
	char buf[1 << 16];
	size_t bc = 0, be = 0;
	char operator()() {
		if (bc >= be) {
			buf[0] = 0, bc = 0;
			be = fread(buf, 1, sizeof(buf), stdin);
		}
		return buf[bc++]; // returns 0 on EOF
	}
} gc;
int read_int() {
	int a, c;
	while ((a = gc()) < 40);
	if (a == '-') return -read_int();
	while ((c = gc()) >= 48) a = a * 10 + c - 480;
	return a - 48;
}

constexpr int BUF_SIZE = sizeof(gc.buf);

string tempdirname;
string tempfilename;

void test(const string& s, vector<int> ints = {}) {
	gc.bc = gc.be = 0;
	ofstream fout(tempfilename);
	fout << s;
	fout.close();
	FILE* ret = freopen(tempfilename.c_str(), "r", stdin);
	assert(ret == stdin);
	if (ints.empty()) {
		for (char c : s) {
			int c2 = gc();
			assert(c == c2);
		}
		assert(gc() == 0);
		assert(gc() == 0);
	} else {
		for (int x : ints) {
			int y = read_int();
			if (x != y) {
				cerr << "on input " << s << ", read " << y << " but expected " << x << endl;
			}
			assert(x == y);
		}
	}
}

int main() {
	// unit test, not stress test, but oh well.
	char pattern[] = "/tmp/fast_input_XXXXXX";
	tempdirname = mkdtemp(pattern);
	tempfilename = tempdirname + "/stdin.txt";

	// first test that the getchar implementation is correct:
	test("");
	test("a");
	test("ab");
	string s;
	for (int i = 0; i < BUF_SIZE; i++) s += (char)(i % 13);
	test(s);
	for (int i = 0; i < BUF_SIZE * 10 + 1; i++) s += (char)(i % 13);
	test(s);
	for (int i = 0; i < BUF_SIZE - 2; i++) s += (char)(i % 13);
	test(s);
	for (int i = 0; i < BUF_SIZE + 2; i++) {
		assert(gc() == 0);
	}

	// then test that read_int() is:
	test("1", {1});
	test("12", {12});
	test("9\n", {9});
	test("12 ", {12});
	test("-23\n", {-23});
	test(" -4", {-4});
	test(" 5\n", {5});
	test("1 -2 ", {1, -2});
	test("  -34   56   ", {-34, 56});
	test(" \t\r\n5 -2 ", {5});

	unlink(tempfilename.c_str());
	rmdir(tempdirname.c_str());
	cout << "tests passed!" << endl;
}
