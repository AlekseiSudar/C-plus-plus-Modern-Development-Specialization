#include "sum_reverse_sort.h"
#include <algorithm>
using namespace std;

int Sum(int x, int y) {
	return x + y;
}

string Reverse(string s) {
	for (int i = s.size() - 2; i >= 0; --i) {
		s.push_back(s[i]);
		s.erase(s.begin() + i);
	}
	return s;
}

void Sort(vector<int>& nums) {
	sort(nums.begin(), nums.end());
}
