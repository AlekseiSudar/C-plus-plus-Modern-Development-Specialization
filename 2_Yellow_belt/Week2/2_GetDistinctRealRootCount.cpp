#include <sstream>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

template<class T, class U>
void AssertEqual(const T& t, const U& u,
	const string& hint)
{
	if (t != u) {
		ostringstream os;
		os << "Assertion failed: " << t << " != " << u
			<< " hint: " << hint;
		throw runtime_error(os.str());
	}
}

inline void Assert(bool b, const string& hint) {
	AssertEqual(b, true, hint);
}

class TestRunner {
public:
	template <class TestFunc>
	void RunTest(TestFunc func, const string& test_name) {
		try {
			func();
			cerr << test_name << " OK" << endl;
		}
		catch (runtime_error& e) {
			++fail_count;
			cerr << test_name << " fail: " << e.what() << endl;
		}
	}

	~TestRunner() {
		if (fail_count > 0) {
			cerr << fail_count << " unit tests failed. Terminate" << endl;
			exit(1);
		}
	}

private:
	int fail_count = 0;
};


void TestAis0() {
	int a = 0, b = 1, c = -1;
	AssertEqual(GetDistinctRealRootCount(a, b, c), 1, "a=0, res!=1");
}
void TestBis0() {
	int a = 1, b = 0, c = -1;
	AssertEqual(GetDistinctRealRootCount(a, b, c), 2, "b=0, res!=2");
}
void TestDis0() {
	int a = 1, b = 2, c = 1;
	AssertEqual(GetDistinctRealRootCount(a, b, c), 1, "D=0, res!=1");
}
void TestDnot0() {
	int a = 1, b = 2, c = -1;
	AssertEqual(GetDistinctRealRootCount(a, b, c), 2, "D!=0, res!=2");
}
void TestDless0() {
	int a = 5, b = 2, c = 1;
	AssertEqual(GetDistinctRealRootCount(a, b, c), 0, "D<0, res!=0");
}
void TestABis0() {
	int a = 0, b = 0, c = -1;
	AssertEqual(GetDistinctRealRootCount(a, b, c), 0, "AB=0, res!=0");
}


void TestAll() {
	TestRunner tr;
	tr.RunTest(TestAis0, "TestAis0");
	tr.RunTest(TestBis0, "TestBis0");
	tr.RunTest(TestDis0, "TestDis0");
	tr.RunTest(TestDnot0, "TestDnot0");
	tr.RunTest(TestDless0, "TestDless0");
	tr.RunTest(TestABis0, "TestABis0");
}int main() {
	TestAll();	return 0;}