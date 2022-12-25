#include "phone_number.h"
#include <iostream>
#include <sstream>
#include <exception>
using namespace std;

PhoneNumber::PhoneNumber(const string& international_number) {
    stringstream stream(international_number);
    string buf;
    if (stream) {
        getline(stream, buf, '+');
        if (buf != "")
            throw invalid_argument("First simbol not +");
        getline(stream, country_code_, '-');
        if (country_code_  == "")
            throw invalid_argument("No country code");
        getline(stream, city_code_, '-');
        if (city_code_ == "")
            throw invalid_argument("No city code");
        getline(stream, local_number_);
        if (local_number_ == "")
            throw invalid_argument("No local number");
    }

}

string PhoneNumber::GetCountryCode() const {
    return country_code_;
}
string PhoneNumber::GetCityCode() const {
    return city_code_;
}
string PhoneNumber::GetLocalNumber() const {
    return local_number_;
}
string PhoneNumber::GetInternationalNumber() const {
    return "+" + country_code_ + "-" + city_code_ + "-" + local_number_;
}