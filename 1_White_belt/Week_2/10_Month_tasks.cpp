#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	int month = 0;
	vector<int> months = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	vector<vector<string>> tasks(31);

	int num_comm;
	cin >> num_comm;
	for (int i = 0; i < num_comm; ++i)
	{
		string comm1;
		cin >> comm1;
		if (comm1 == "NEXT")
		{
			int pre_month = month;
			if (month == 11)
				month = 0;
			else
				++month;


			for (int i = months[month]; i < months[pre_month]; ++i)
			{
				tasks[months[month]-1].insert(
					end(tasks[months[month]-1]),
					begin(tasks[i]),
					end(tasks[i]));
			}


			tasks.resize(months[month]);
		}
		else
		{
			int comm2;
			cin >> comm2;
			if (comm1 == "DUMP")
			{
				cout << tasks[comm2-1].size() << " ";
				for (int i = 0; i < tasks[comm2-1].size(); ++i)
				{
					cout << tasks[comm2-1][i] << " ";
				}
				cout << endl;
			}
			else if (comm1 == "ADD")
			{
				string comm3;
				cin >> comm3;
				tasks[comm2-1].push_back(comm3);
			}
		}
	}

}
