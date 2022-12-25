#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void PrintStats(vector<Person> persons){
    auto begin = persons.begin();
    auto end = persons.end();
    auto gender = partition(begin, end, [](const Person& person){ return person.gender == Gender::MALE; });
    auto male_is_employed = partition(begin, gender, [](const Person& person){ return person.is_employed; });
    auto female_is_employed = partition(gender, end, [](const Person& person){ return person.is_employed; });

    cout << "Median age = "                        << ComputeMedianAge(begin, end) << endl
         << "Median age for females = "            << ComputeMedianAge(gender, end) << endl
         << "Median age for males = "              << ComputeMedianAge(begin, gender) << endl
         << "Median age for employed females = "   << ComputeMedianAge(gender, female_is_employed) << endl
         << "Median age for unemployed females = " << ComputeMedianAge(female_is_employed, end) << endl
         << "Median age for employed males = "     << ComputeMedianAge(begin, male_is_employed) << endl
         << "Median age for unemployed males = "   << ComputeMedianAge(male_is_employed, gender) << endl;
}