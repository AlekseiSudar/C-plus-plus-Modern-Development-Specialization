#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;

void PrintVector(const vector<string>& vect)
{
	for (const auto& i : vect)
		cout << i << " ";
	cout << endl;
}

int main()
{
	int n;
	cin >> n;

	vector<string> buses;
	vector<vector<string>> stops;
	map<string, vector<string>> bus_stops;

	for (int i = 0; i < n; ++i)
	{
		string command;
		cin >> command;

		if (command == "NEW_BUS")
		{
			string bus;
			int stops_num;
			cin >> bus >> stops_num;

			buses.push_back(bus);
			stops.push_back({});
			for (int i = 0; i < stops_num; ++i)
			{
				string stop;
				cin >> stop;
				stops.back().push_back(stop);

				bus_stops[bus].push_back(stop);
			}
		}
		else if (command == "BUSES_FOR_STOP")
		{
			string stop;
			cin >> stop;

			vector<string> buses_for_stop;
			for (int i = 0; i < stops.size(); ++i)
			{
				for (int j = 0; j < stops[i].size(); ++j)
					if (stops[i][j] == stop)
					{
						buses_for_stop.push_back(buses[i]);
						break;
					}
			}

			if (buses_for_stop.size())
				PrintVector(buses_for_stop);
			else
				cout << "No stop" << endl;
		}
		else if (command == "STOPS_FOR_BUS")
		{
			string bus;
			cin >> bus;
 
			if (bus_stops.count(bus))
			{
				for (const auto& stop : bus_stops[bus])
				{
					cout << "Stop " << stop << ": ";
					int i = 0;
					for (int b = 0; b < buses.size(); ++b)
					{
						if (buses[b] != bus)
						{
							for (int s = 0; s < stops[b].size(); ++s)
							{
								if (stops[b][s] == stop)
								{
									cout << buses[b] << " ";
									++i;
									break;
								}
							}
						}
					}
					if (i == 0)
						cout << "no interchange";
					cout << endl;
				}
			}
			else
				cout << "No bus" << endl;
		}
		else if (command == "ALL_BUSES")
		{
			if (buses.size())
			{
				
				for (int i = 0; i < buses.size(); ++i)
					bus_stops[buses[i]] = stops[i];

				for (const auto& bus : bus_stops)
				{
					cout << "Bus " << bus.first << ": ";
					PrintVector(bus.second);
				}
			}
			else
				cout << "No buses" << endl;
		}
	}
}
