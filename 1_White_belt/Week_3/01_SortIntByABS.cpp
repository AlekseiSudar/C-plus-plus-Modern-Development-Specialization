#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	int n;
	cin >> n;

	vector<int> nums(n);
	for (int i = 0; i < n; ++i)
		cin >> nums[i];

	sort(nums.begin(), nums.end(), [](int i, int j) { return abs(i) < abs(j); });

	for (int i = 0; i < n; ++i)
		cout << nums[i] << " ";
}
