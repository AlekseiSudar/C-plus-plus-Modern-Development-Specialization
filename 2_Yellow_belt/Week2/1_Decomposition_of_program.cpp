#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
  NewBus,
  BusesForStop,
  StopsForBus,
  AllBuses
};

struct Query {
  QueryType type;
  string bus;
  string stop;
  vector<string> stops;
};

istream& operator >> (istream& is, Query& q) {
	string type;
	is >> type;
	if (type == "NEW_BUS") {
		q.type = QueryType::NewBus;
		is >> q.bus;
		
		int stops_count;
		is >> stops_count;

		q.stops.resize(stops_count);
		for (string& stop : q.stops) {
			cin >> stop;
		}
	}
	else if (type == "BUSES_FOR_STOP") {
		q.type = QueryType::BusesForStop;
		is >> q.stop;
	}
	else if (type == "STOPS_FOR_BUS") {
		q.type = QueryType::StopsForBus;
		is >> q.bus;
	}
	else if (type == "ALL_BUSES") {
		q.type = QueryType::AllBuses;
	}
	return is;
}

struct BusesForStopResponse {
	vector<string> buses;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
	if (r.buses.empty()) {
		cout << "No stop" << endl;
	}
	else {
		for (const string& bus : r.buses) {
			cout << bus << " ";
		}
		cout << endl;
	}
	return os;
}

struct StopsForBusResponse {
	string bus;
	vector<string> stops;
	map<string, vector<string>> buses_for_stops;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
	if (r.stops.empty()) {
		cout << "No bus" << endl;
	}
	else {
		for (const string& stop : r.stops) {
			cout << "Stop " << stop << ": ";
			if (r.buses_for_stops.at(stop).size() == 1) {
				cout << "no interchange";
			}
			else {
				for (const string& other_bus : r.buses_for_stops.at(stop)) {
					if (r.bus != other_bus) {
						cout << other_bus << " ";
					}
				}
			}
			cout << endl;
		}
	}
	return os;
}

struct AllBusesResponse {
	map<string, vector<string>> buses;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
	if (r.buses.empty()) {
		cout << "No buses" << endl;
	}
	else {
		for (const auto& bus_item : r.buses) {
			cout << "Bus " << bus_item.first << ": ";
			for (const string& stop : bus_item.second) {
				cout << stop << " ";
			}
			cout << endl;
		}
	}
	return os;
}

class BusManager {
	map<string, vector<string>> buses_for_stops, stops_for_buses;
public:
	void AddBus(const string& bus, const vector<string>& stops) {
		stops_for_buses[bus] = stops;
		for (string stop : stops) {
			buses_for_stops[stop].push_back(bus);
		}
	}
	BusesForStopResponse GetBusesForStop(const string& stop) const {
		if (buses_for_stops.count(stop))
			return { buses_for_stops.at(stop) };
		else
			return {};
	}
	StopsForBusResponse GetStopsForBus(const string& bus) const {
		if (stops_for_buses.count(bus)) {
			return { bus, stops_for_buses.at(bus), buses_for_stops };
		}
		else
			return {};
	}
	AllBusesResponse GetAllBuses() const {
		return { stops_for_buses };
	}
};

// Не меняя тела функции main, реализуйте функции и классы выше

int main() {
  int query_count;
  Query q;

  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
    case QueryType::NewBus:
      bm.AddBus(q.bus, q.stops);
      break;
    case QueryType::BusesForStop:
      cout << bm.GetBusesForStop(q.stop) << endl;
      break;
    case QueryType::StopsForBus:
      cout << bm.GetStopsForBus(q.bus) << endl;
      break;
    case QueryType::AllBuses:
      cout << bm.GetAllBuses() << endl;
      break;
    }
  }

  return 0;
}