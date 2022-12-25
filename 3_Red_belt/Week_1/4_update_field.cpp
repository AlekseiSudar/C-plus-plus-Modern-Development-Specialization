#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

// Реализуйте этот макрос, а также необходимые операторы для классов Date и Time
#define UPDATE_FIELD(ticket, field, values) \
if (values.count(#field)){                  \
    istringstream is(values.at(#field));    \
    is >> ticket.field;                     \
}

istream& operator>>(istream& is, Date& date) {
    int year, month, day;
    is >> year;
    is.ignore(1);
    is >> month;
    is.ignore(1);
    is >> day;
    is.ignore(1);
    date = { year, month, day };
    return is;
}
istream& operator>>(istream& is, Time& time) {
    int hours, minutes;
    is >> hours;
    is.ignore(1);
    is >> minutes;
    is.ignore(1);
    time = { hours, minutes };
    return is;
}


ostream& operator << (ostream& os, const Date date) {
    os << date.year << '-' << date.month << '-' << date.day;
    return os;
}
ostream& operator << (ostream& os, const Time time) {
    os << time.hours << ":" << time.minutes;
    return os;
}

bool operator==(const Date& lhs, const Date& rhs) {
    if (lhs.day == rhs.day && lhs.month == rhs.month && lhs.year == rhs.year){
        return true;
    }
    return false;
}
bool operator==(const Time& lhs, const Time& rhs) {
    if (lhs.minutes == rhs.minutes && lhs.hours == rhs.hours){
        return true;
    }
    return false;
}

void TestUpdate() {
    AirlineTicket t;
    t.price = 0;

    const map<string, string> updates1 = {
    {"departure_date", "2018-2-28"},
    {"departure_time", "17:40"},
    };
    UPDATE_FIELD(t, departure_date, updates1);
    UPDATE_FIELD(t, departure_time, updates1);
    UPDATE_FIELD(t, price, updates1);

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 0);

    const map<string, string> updates2 = {
    {"price", "12550"},
    {"arrival_time", "20:33"},
    };
    UPDATE_FIELD(t, departure_date, updates2);
    UPDATE_FIELD(t, departure_time, updates2);
    UPDATE_FIELD(t, arrival_time, updates2);
    UPDATE_FIELD(t, price, updates2);

    // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
    // значения этих полей не должны измениться
    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 12550);
    ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}
/*
int main() {
  TestRunner tr;
  RUN_TEST(tr, TestUpdate);
}
*/