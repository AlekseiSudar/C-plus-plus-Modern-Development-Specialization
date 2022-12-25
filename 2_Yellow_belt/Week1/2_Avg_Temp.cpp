#include <iostream>
#include <vector>
using namespace std;

int main() {
	int n;
	cin >> n;
	vector<int> temps(n);
	int64_t sum_temp = 0;
	for (auto& temp : temps) {
		cin >> temp;
		sum_temp += temp;
	}
	int avg = sum_temp / n;

	vector<int> days;
	for (int i = 0; i < n; ++i) {
		if (temps[i] > avg)
			days.push_back(i);
	}

	cout << days.size() << endl;
	for (auto& i : days)
		cout << i << " ";

	return 0;
}