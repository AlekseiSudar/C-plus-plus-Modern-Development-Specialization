#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

void PrintStats(vector<Person> persons) {
	auto first = persons.begin();
	auto last = persons.end();
	cout << "Median age = " << ComputeMedianAge(first, last) << endl;

	auto it_gender = partition(first, last,
		[](const Person& person) {return person.gender == Gender::FEMALE; });
	cout << "Median age for females = " << ComputeMedianAge(first, it_gender) << endl;
	cout << "Median age for males = " << ComputeMedianAge(it_gender, last) << endl;

	auto it_females_empl = partition(first, it_gender,
		[](const Person& person) {return person.is_employed; });
	cout << "Median age for employed females = " << ComputeMedianAge(first, it_females_empl) << endl;
	cout << "Median age for unemployed females = " << ComputeMedianAge(it_females_empl, it_gender) << endl;

	auto it_males_empl = partition(it_gender, last,
		[](const Person& person) {return person.is_employed; });
	cout << "Median age for employed males = " << ComputeMedianAge(it_gender, it_males_empl) << endl;
	cout << "Median age for unemployed males = " << ComputeMedianAge(it_males_empl, last) << endl;
}
