#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
	os << "{";
	bool first = true;
	for (const auto& x : s) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
	os << "{";
	bool first = true;
	for (const auto& x : s) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
	os << "{";
	bool first = true;
	for (const auto& kv : m) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << kv.first << ": " << kv.second;
	}
	return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
	if (t != u) {
		ostringstream os;
		os << "Assertion failed: " << t << " != " << u;
		if (!hint.empty()) {
			os << " hint: " << hint;
		}
		throw runtime_error(os.str());
	}
}

void Assert(bool b, const string& hint) {
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
		catch (exception& e) {
			++fail_count;
			cerr << test_name << " fail: " << e.what() << endl;
		}
		catch (...) {
			++fail_count;
			cerr << "Unknown exception caught" << endl;
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

void TestEmpty() {
	AssertEqual(IsPalindrom(""), true, "Empty srting not palindrom");
}

void TestOne() {
	AssertEqual(IsPalindrom("a"), true, "\"a\" not palindrom");
	AssertEqual(IsPalindrom(" "), true, "\" \" not palindrom");
	AssertEqual(IsPalindrom("  "), true, "\"  \" not palindrom");
	AssertEqual(IsPalindrom("     "), true, "\"     \" not palindrom");
}

void TestSimple() {
	AssertEqual(IsPalindrom("asdfdsa"), true, "\"asdfdsa\" not palindrom");
	AssertEqual(IsPalindrom("asdffdsa"), true, "\"asdffdsa\" not palindrom");
}

void TestSpase() {
	AssertEqual(IsPalindrom("as dfd sa"), true, "\"as dfd sa\" not palindrom");
	AssertEqual(IsPalindrom("as dffd sa"), true, "\"as dffd sa\" not palindrom");
	AssertEqual(IsPalindrom("as dfdd sa"), false, "\"as dfdd sa\" is palindrom");
	AssertEqual(IsPalindrom(" as dffd sa"), false, "\" as dffd sa\" is palindrom");
	AssertEqual(IsPalindrom("as dffd sa "), false, "\"as dffd sa \" is palindrom");
	AssertEqual(IsPalindrom("  as dffd sa"), false, "\"  as dffd sa\" is palindrom");
	AssertEqual(IsPalindrom("as dffd sa  "), false, "\"as dffd sa  \" is palindrom");
	AssertEqual(IsPalindrom("  as dffd sa "), false, "\"  as dffd sa \" is palindrom");
	AssertEqual(IsPalindrom(" as dffd sa  "), false, "\" as dffd sa  \" is palindrom");
	AssertEqual(IsPalindrom("  as  dffd sa "), false, "\"  as  dffd sa \" is palindrom");
	AssertEqual(IsPalindrom(" as dffd  sa  "), false, "\" as dffd  sa  \" is palindrom");
	AssertEqual(IsPalindrom("As dffd sa"), false, "\"As dffd sa\" is palindrom");
	AssertEqual(IsPalindrom("as dffd sA"), false, "\"as dffd sA\" is palindrom");
	AssertEqual(IsPalindrom("\nas dffd sA"), false, "\"\\nas dffd sA\" is palindrom");
	AssertEqual(IsPalindrom("as dffd sA\n"), false, "\"as dffd sA\\n\" is palindrom");
}

int main() {
	TestRunner runner;
	runner.RunTest(TestEmpty, "TestEmpty");
	runner.RunTest(TestOne, "TestOne");
	runner.RunTest(TestSimple, "TestSimple");
	runner.RunTest(TestSpase, "TestSpase");
	return 0;
}