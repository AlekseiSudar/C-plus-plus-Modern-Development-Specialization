#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <map>
#include <sstream>
using namespace std;


class Date {
public:
	Date() {
		year = 0;
		month = 0;
		day = 0;
	}
	Date(const int& y, const int& m, const int& d) {
		year = y;

		if (m < 1 || m > 12)
			throw runtime_error("Month value is invalid: " + to_string(m));
		month = m;

		if (d < 1 || d > 31)
			throw runtime_error("Day value is invalid: " + to_string(d));
		day = d;
	}
	int GetYear() const { return year; }
	int GetMonth() const { return month; }
	int GetDay() const { return day; }
private:
	int year;
	int month;
	int day;
};
bool operator<(const Date& lhs, const Date& rhs) {
	if (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth())
		return lhs.GetDay() < rhs.GetDay();
	else if (lhs.GetYear() == rhs.GetYear())
		return lhs.GetMonth() < rhs.GetMonth();
	else
		return lhs.GetYear() < rhs.GetYear();
}


bool EnsureNextSymbolAndSkip(stringstream& stream) {
	if (stream.peek() == '-') {
		stream.ignore(1);
		return true;
	}
	return false;
}
istream& operator>>(istream& stream, Date& date) {
	string line;
	stream >> line;
	stringstream stream2(line);

	bool b = true;
	int year, month, day;
	if (stream2 >> year)
		if (EnsureNextSymbolAndSkip(stream2))
			if (stream2 >> month)
				if (EnsureNextSymbolAndSkip(stream2))
					if (stream2 >> day)
					{
						string s;
						stream2 >> s;
						if (s.empty())
						{
							date = { year,month,day };
							return stream;
						}
					}
	throw runtime_error("Wrong date format: " + line);
}


class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		events[date].insert(event);
	}
	bool DeleteEvent(const Date& date, const string& event) {
		if (events[date].count(event)) {
			events[date].erase(event);
			return true;
		}
		return false;
	}
	int  DeleteDate(const Date& date) {
		int n = events[date].size();
		events.erase(date);
		return n;
	}
	void Find(const Date& date) const {
		if (events.count(date))
			for (const auto& i : events.at(date))
				cout << i << endl;
	}
	void Print() const {
		for (const auto& [key, value] : events)
		{
			for (const auto& i : value)
			{
				cout << setfill('0');
				cout << setw(4) << key.GetYear() << '-'
					<< setw(2) << key.GetMonth() << '-'
					<< setw(2) << key.GetDay() << ' '
					<< i << endl;
			}
		}

	}
private:
	map<Date, set<string>> events;
};


void Comm(Database& DB) {
	string line;

	while (getline(cin, line)) {
		stringstream stream(line);
		string comm;
		stream >> comm;

		if (comm == "Add") {
			Date date;
			string event;
			stream >> date >> event;
			if (!event.empty())
				DB.AddEvent(date, event);
		}
		else if (comm == "Del") {
			Date date;
			string event;
			stream >> date >> event;
			if (event.empty()) {
				cout << "Deleted " << DB.DeleteDate(date) << " events" << endl;
			}
			else {
				bool i = DB.DeleteEvent(date, event);
				if (i)
					cout << "Deleted successfully" << endl;
				else
					cout << "Event not found" << endl;
			}
		}
		else if (comm == "Find") {
			Date date;
			if (stream >> date)
				DB.Find(date);
		}
		else if (comm == "Print") {
			DB.Print();
		}
		else if (comm.empty()) {
			
		}
		else
		{
			throw runtime_error("Unknown command: " + comm);
		}
	}
}


int main() {
	Database DB;
	try {
		Comm(DB);
	}
	catch (runtime_error& re ) {
		cout << re.what();
	}
	return 0;
}
