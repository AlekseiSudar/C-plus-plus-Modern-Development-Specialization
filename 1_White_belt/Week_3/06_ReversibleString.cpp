#include <iostream>
#include <string>
using namespace std;

class ReversibleString {
public:
	ReversibleString() {
		s = "";
	}
	ReversibleString(const string& s_in) {
		s = s_in;
	}

	string ToString() const {
		return s;
	}
	void Reverse() {
		for (int i = s.size() - 2; i >= 0; --i) {
			s.push_back(s[i]);
			s.erase(s.begin() + i);
		}
	}
private:
	string s;
};