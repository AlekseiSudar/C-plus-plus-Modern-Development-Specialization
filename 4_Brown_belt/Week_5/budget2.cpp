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
    BudgetManager() : income_(SIZE_OF_MANAGER), expenses_(SIZE_OF_MANAGER) {}
    double ComputeIncome(const Date& from, const Date& to){
        double income = 0.0;
        for (auto it1 = income_.begin() + ComputeDay(from)
                , ite = expenses_.begin() + ComputeDay(from)
                , it2 = income_.begin() + ComputeDay(to) + 1
                ; it1 != it2; it1++, ite++){
            income += *it1 - *ite;
        }
        return income;
    }
    void Earn(const Date& from, const Date& to, double income){
        income /= (ComputeDaysDiff(to, from) + 1);
        for (auto it1 = income_.begin() + ComputeDay(from)
                , it2 = income_.begin() + ComputeDay(to) + 1
                ; it1 != it2; it1++){
            *it1 += income;
        }
    }
    void PayTax(const Date& from, const Date& to, double percentage){
        for (auto it1 = income_.begin() + ComputeDay(from)
                , it2 = income_.begin() + ComputeDay(to) + 1
                ; it1 != it2; it1++){
            *it1 *= 1.0 - percentage / 100.0;
        }
    }
    void Spend(const Date& from, const Date& to, double expenses){
        expenses /= (ComputeDaysDiff(to, from) + 1);
        for (auto it1 = expenses_.begin() + ComputeDay(from)
                , it2 = expenses_.begin() + ComputeDay(to) + 1
                ; it1 != it2; it1++){
            *it1 += expenses;
        }
    }
private:
    size_t ComputeDay(const Date& date){
        return ComputeDaysDiff(date, START_DATE);
    }
    const static inline Date START_DATE = Date("2000-01-01");
    const static inline size_t SIZE_OF_MANAGER = ComputeDaysDiff(Date("2100-01-01"), START_DATE);
    vector<double> income_;
    vector<double> expenses_;
};

int main(){
    cout.precision(25);

    BudgetManager manager;

    size_t num_queries;
    cin >> num_queries;

    string command;
    Date from, to;
    double value;

    for (size_t i = 0; i < num_queries; i++){
        cin >> command >> from >> to;
        if (command == "ComputeIncome"){
            cout << manager.ComputeIncome(from, to) << endl;
        } else if (command == "PayTax"){
            cin >> value;
            manager.PayTax(from, to, value);
        } else if (command == "Earn"){
            cin >> value;
            manager.Earn(from, to, value);
        } else if (command == "Spend"){
            cin >> value;
            manager.Spend(from, to, value);
        }
    }

    return 0;
}