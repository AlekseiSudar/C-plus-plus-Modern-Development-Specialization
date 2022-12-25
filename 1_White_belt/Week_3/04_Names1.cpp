#include <iostream>
#include <map>
#include <string>
using namespace std;

class Person {
public:
	void ChangeFirstName(int year, const string& first_name) {
		firstName[year] = first_name;
	}
	void ChangeLastName(int year, const string& last_name) {
		lastName[year] = last_name;
	}
	string GetFullName(int year) {
		string fn;
		string ln;
		
		for (const auto& i : firstName)
			if (i.first <= year)
				fn = i.second;
			else
				break;
		for (const auto& i : lastName)
			if (i.first <= year)
				ln = i.second;
			else
				break;
		if (fn.size() > 0 && ln.size() > 0)
			return fn + " " + ln;
		else if (fn.size() > 0)
			return fn + " with unknown last name";
		else if (ln.size() > 0)
			return ln + " with unknown first name";
		else
			return "Incognito";
	}
private:
	map<int, string> firstName;
	map<int, string> lastName;
};