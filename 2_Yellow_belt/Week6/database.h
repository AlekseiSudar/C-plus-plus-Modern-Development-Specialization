#pragma once
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <memory>
#include "date.h"
#include "events.h"

using namespace std;

ostream& operator<<(ostream& os, const  pair<Date, string>& pair);

class Database
{
public:
	void Add(const Date& date, const string& event) {
		data[date].Add(event);
	}
	

	void Print(ostream& os) const {
		for (const auto&[key, value] : data) {
			for (const auto& i : value.GetSet()) {
				os << make_pair(key, i) << endl;
			}
		}
	}

	template<typename function>
	int RemoveIf(function f) {
		int n = 0;
		
		for (auto& i : data) {
			const Date& date = i.first;
			n += i.second.RemoveIf([=](const string& event) {
				return f(date, event);
			});
		}
		for (auto it = data.begin(); it != data.end(); ) {
			if (it->second.GetSet().empty()) {
				data.erase(it++);
			}
			else {
				++it;
			}
		}
		
		return n;
	}

	template<typename function>
	vector<pair<Date, string>> FindIf(function f) const {
		vector<pair<Date, string>> out;

		for (const auto&[key, value] : data) {
			for (const auto& i : value.GetSet()) {
				if (f(key, i)) {
					out.push_back(make_pair(key, i));
				}
			}
		}

		return out;
	}

	pair<Date, string> Last(Date date) const {
		auto it = data.upper_bound(date);		if (it == data.begin()) {			throw invalid_argument("");		}		it--;
		return make_pair(it->first, it->second.GetVector().back());
	}

private:
	map<Date, Events> data;
};
