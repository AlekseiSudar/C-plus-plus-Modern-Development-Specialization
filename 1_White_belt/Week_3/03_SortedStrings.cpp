#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class SortedStrings {
private:
	vector<string> strings;
public:
    void AddString(const string& s) {
        strings.push_back(s);
		sort(strings.begin(), strings.end());
    }
    vector<string> GetSortedStrings() {
		return strings;
    }
};