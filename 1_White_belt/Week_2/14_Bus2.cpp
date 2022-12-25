#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;

int main()
{
	int n;
	cin >> n;

	map<vector<string>, int> stops_num;

	for (int i = 0; i < n; ++i)
	{
		int j;
		cin >> j;

		vector<string> stops;
		for (int k = 0; k < j; ++k)
		{
			string stop;
			cin >> stop;
			stops.push_back(stop);
		}

		if (stops_num.count(stops))
			cout << "Already exists for " << stops_num[stops] << endl;
		else
		{
			stops_num[stops] = stops_num.size() + 1;
			cout << "New bus " << stops_num.size() << endl;
		}
	}
}
