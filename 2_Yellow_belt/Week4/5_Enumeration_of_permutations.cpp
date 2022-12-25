#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

void my_cout(vector<vector<int>>& nums) {
	for (int i = 0, n = nums.size(); i < n; ++i) {
		for (int j = 0, m = nums[i].size(); j < m; ++j) {
			cout << nums[i][j] << ' ';
		}
		cout << endl;
	}
}

int main() {
	int n;
	cin >> n;

	vector<int> permut;
	while (n != 0) {
		permut.push_back(n--);
	}

	vector<vector<int>> permuts;
	permuts.push_back(permut);
	bool b = prev_permutation(permut.begin(), permut.end());

	while (b) {
		permuts.push_back(permut);
		b = prev_permutation(permut.begin(), permut.end());
	}

	my_cout(permuts);

}