#include <iostream>
#include <vector>
using namespace std;

int main()
{
	int n;
	cin >> n;

	vector<int> temps;
	int temps_sum = 0;
	for (int i = 0; i < n; ++i)
	{
		int t;
		cin >> t;
		temps.push_back(t);
		temps_sum += t;
	}
	temps_sum /= n;

	vector<int> temps_res;
	for (int i = 0; i < temps.size(); ++i)
		if (temps[i] > temps_sum)
			temps_res.push_back(i);

	cout << temps_res.size() << endl;
	for (auto i : temps_res)
		cout << i << " ";

	return 0;
}