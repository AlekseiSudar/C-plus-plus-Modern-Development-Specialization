#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Date{
public:
    Date(){}
    Date(string date)
        : year_(stoi(date.substr(0,4)))
        , month_(stoi(date.substr(5,2)))
        , day_(stoi(date.substr(8,2))){}
    
    time_t AsTimestamp() const {
        std::tm t;
        t.tm_sec   = 0;
        t.tm_min   = 0;
        t.tm_hour  = 0;
        t.tm_mday  = day_;
        t.tm_mon   = month_ - 1;
        t.tm_year  = year_ - 1900;
        t.tm_isdst = 0;
        return mktime(&t);
    }
private:
    int year_, month_, day_;
};

istream& operator>>(istream& is, Date& date){
    string date_string;
    is >> date_string;
    date = Date(move(date_string));
    return is;
}

size_t ComputeDaysDiff(const Date& date_to, const Date& date_from) {
    const time_t timestamp_to = date_to.AsTimestamp();
    const time_t timestamp_from = date_from.AsTimestamp();
    static const int SECONDS_IN_DAY = 60 * 60 * 24;
    return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}

class BudgetManager{
public:
    BudgetManager() : money_(SIZE_OF_MANAGER) {}
    double ComputeIncome(const Date& from, const Date& to){
        double income = 0.0;
        for (auto it1 = money_.begin() + ComputeDay(from)
                , it2 = money_.begin() + ComputeDay(to) + 1
                ; it1 != it2; it1++){
            income += *it1;
        }
        return income;
    }
    void Earn(const Date& from, const Date& to, double income){
        income /= (ComputeDaysDiff(to, from) + 1);
        for (auto it1 = money_.begin() + ComputeDay(from)
                , it2 = money_.begin() + ComputeDay(to) + 1
                ; it1 != it2; it1++){
            *it1 += income;
        }
    }
    void PayTax(const Date& from, const Date& to){
        for (auto it1 = money_.begin() + ComputeDay(from)
                , it2 = money_.begin() + ComputeDay(to) + 1
                ; it1 != it2; it1++){
            *it1 *= TAX_coeff;
        }
    }
private:
    size_t ComputeDay(const Date& date){
        return ComputeDaysDiff(date, START_DATE);
    }
    constexpr static inline double TAX_coeff = 0.87;
    const static inline Date START_DATE = Date("2000-01-01");
    const static inline size_t SIZE_OF_MANAGER = ComputeDaysDiff(Date("2100-01-01"), START_DATE);
    vector<double> money_;
};

int main(){
    cout.precision(25);

    BudgetManager manager;

    size_t num_queries;
    cin >> num_queries;

    string command;
    Date from, to;
    double income;

    for (size_t i = 0; i < num_queries; i++){
        cin >> command >> from >> to;
        if (command == "ComputeIncome"){
            income = manager.ComputeIncome(from, to);
            cout << income << endl;
        } else if (command == "PayTax"){
            manager.PayTax(from, to);
        } else if (command == "Earn"){
            cin >> income;
            manager.Earn(from, to, income);
        }
    }

    return 0;
}