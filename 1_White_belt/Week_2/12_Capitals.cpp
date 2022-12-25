#include <iostream>
#include <map>
#include <string>
using namespace std;

int main()
{
	int n;
	cin >> n;

	map<string, string> capitals;

	for (int i = 0; i < n; ++i)
	{
		string comm1;
		cin >> comm1;

		if (comm1 == "DUMP")
		{
			if (capitals.size() == 0)
				cout << "There are no countries in the world" << endl;
			else
			{
				for (auto i : capitals)
					cout << i.first << "/" << i.second << " ";
				cout << endl;
			}
		}
		else
		{
			string comm2;
			cin >> comm2;

			if (comm1 == "ABOUT")
			{
				if (capitals.count(comm2))
					cout << "Country " << comm2 << " has capital " << capitals[comm2] << endl;
				else
					cout << "Country " << comm2 << " doesn't exist" << endl;
			}
			else
			{
				string comm3;
				cin >> comm3;
				if (comm1 == "CHANGE_CAPITAL")
				{
					if (!capitals.count(comm2))
					{
						capitals[comm2] = comm3;
						cout << "Introduce new country " << comm2 << " with capital " << comm3 << endl;
					}
					else if (capitals[comm2] == comm3)
					{
						cout << "Country " << comm2 << " hasn't changed its capital" << endl;
					}
					else
					{
						cout << "Country " << comm2 << " has changed its capital from " << capitals[comm2] << " to " << comm3 << endl;
						capitals[comm2] = comm3;
					}
				}
				else if (comm1 == "RENAME")
				{
					if (comm2 == comm3 || !capitals.count(comm2) || capitals.count(comm3))
					{
						cout << "Incorrect rename, skip" << endl;
					}
					else
					{
						capitals[comm3] = capitals[comm2];
						capitals.erase(comm2);
						cout << "Country " << comm2 << " with capital " << capitals[comm3] << " has been renamed to " << comm3 << endl;
					}
				}
			}
		}
	}
}
