#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<string> SplitIntoWords(const string& s) {
	auto it_1 = s.begin();
	auto it_2 = s.begin();
	auto it_end = s.end();

	vector<string> output;
	while (it_2 != it_end) {
		it_2 = find(it_1, it_end, ' ');
		output.push_back({ it_1, it_2 });
		if (it_2 != it_end) {
			it_2++;
			it_1 = it_2;
		}
	}
	return output;
}

int main() {
	string s = "C Cpp Java Python";

	vector<string> words = SplitIntoWords(s);
	cout << words.size() << " ";
	for (auto it = begin(words); it != end(words); ++it) {
		if (it != begin(words)) {
			cout << "/";
		}
		cout << *it;
	}
	cout << endl;

	return 0;
}