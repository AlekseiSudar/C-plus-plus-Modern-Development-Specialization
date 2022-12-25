#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	int n;
	cin >> n;

	vector<string> nums(n);
	for (int i = 0; i < n; ++i)
		cin >> nums[i];

	sort(nums.begin(), nums.end(),[](string i, string j) {
		for (int k = 0; k < min(i.size(),j.size()); ++k)
		{
			i[k] = tolower(i[k]);
			j[k] = tolower(j[k]);
		}
		return i < j; });

	for (int i = 0; i < n; ++i)
		cout << nums[i] << " ";

}
