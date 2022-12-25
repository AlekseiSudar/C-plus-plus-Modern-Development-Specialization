#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <tuple>
using namespace std;


class Date {
public:
	Date(const int& y, const int& m, const int& d)
		:year(y), month(m), day(d) {
		if (m < 1 || m > 12)
			throw invalid_argument("Month value is invalid: " + to_string(m));
		if (d < 1 || d > 31)
			throw invalid_argument("Day value is invalid: " + to_string(d));
	}

	int GetYear() const { return year; }
	int GetMonth() const { return month; }
	int GetDay() const { return day; }

private:
	const int year, month, day;
};


ostream& operator<<(ostream& os, const Date& date);


Date ParseDate(istream& is);

bool operator==(const Date& lhs, const Date& rhs);
bool operator!=(const Date& lhs, const Date& rhs);
bool operator<=(const Date& lhs, const Date& rhs);
bool operator>=(const Date& lhs, const Date& rhs);
bool operator<(const Date& lhs, const Date& rhs);
bool operator>(const Date& lhs, const Date& rhs);