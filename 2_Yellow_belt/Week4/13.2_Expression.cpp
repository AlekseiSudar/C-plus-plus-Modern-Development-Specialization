#include <iostream>
#include <string>
#include <deque>

using namespace std;

int main() {

	deque<string> result(1);
	cin >> result[0];

	int num_oper;
	cin >> num_oper;

	deque<string> nums;
	deque<string> opers;
	for (int i = 0; i < num_oper; ++i) {
		string num;
		string oper;
		cin >> oper >> num;
		nums.push_back(num);
		opers.push_back(oper);
	}

	
	for (int i = 0; i < num_oper; ++i) {
		if (i == 0 || opers[i] == "+" || opers[i] == "-") {
			result.push_back(" ");
			result.push_back(opers[i]);
			result.push_back(" ");
			result.push_back(nums[i]);
		}
		else if (opers[i-1] == "+" || opers[i-1] == "-") {
			result.push_front("(");
			result.push_back(") ");
			result.push_back(opers[i]);
			result.push_back(" ");
			result.push_back(nums[i]);
		}
		else {
			result.push_back(" ");
			result.push_back(opers[i]);
			result.push_back(" ");
			result.push_back(nums[i]);
		}
	}

	for (auto i : result)
		cout << i;
	
	return 0;
}