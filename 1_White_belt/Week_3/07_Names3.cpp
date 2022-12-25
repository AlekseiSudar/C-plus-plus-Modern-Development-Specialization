#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

string GetName (const int& year, const map<int, string>& names)
{
	string name;
	for (const auto& i : names)
		if (i.first <= year)
			name = i.second;
		else
			break;
	return name;
}
vector<string> GetNameHistory(const int& year, const map<int, string>& names)
{
	vector<string> name;
	for (const auto& i : names)
		if (i.first <= year)
			name.push_back(i.second);
		else
			break;
	return name;
}
string PrintHistory(const vector<string>& names)
{
	vector<string> unic_names;
	for (int i = names.size() - 2; i >= 0; --i)
		if (names[i] != names[i + 1])
			unic_names.push_back(names[i]);

	string name;
	if (unic_names.size() > 0)
	{
		name = " (";
		for (int i = 0; i < unic_names.size(); ++i)
		{
			name += unic_names[i];
			if (i < unic_names.size() - 1)
				name += ", ";
		}
		name += ")";
	}
	return name;
}
int GetBirthday(const map<int, string> name) {
	for (const auto& i : name)
		return i.first;
}

class Person {
public:
	Person(const string& first_name, const string& last_name, int year) {
		firstName[year] = first_name;
		lastName[year] = last_name;
	}

	void ChangeFirstName(int year, const string& first_name) {
		if (year >= GetBirthday(firstName))
			firstName[year] = first_name;
	}
	void ChangeLastName(int year, const string& last_name) {
		if (year >= GetBirthday(firstName))
			lastName[year] = last_name;
	}
	string GetFullName (int year) const {
		string fn = GetName(year, firstName);
		string ln = GetName(year, lastName);

		if (year < GetBirthday(firstName))
			return "No person";
		else if (fn.size() > 0 && ln.size() > 0)
			return fn + " " + ln;
		else if (fn.size() > 0)
			return fn + " with unknown last name";
		else if (ln.size() > 0)
			return ln + " with unknown first name";
		else
			return "Incognito";
	}
	string GetFullNameWithHistory(int year) const {
		vector<string> fn_h = GetNameHistory(year, firstName);
		vector<string> ln_h = GetNameHistory(year, lastName);

		string name;
		if (year < GetBirthday(firstName))
			return "No person";
		else if (fn_h.size() > 0 && ln_h.size() > 0)
			return fn_h.back() + PrintHistory(fn_h) + " " + ln_h.back() + PrintHistory(ln_h);
		else if (fn_h.size() > 0)
			return fn_h.back() + PrintHistory(fn_h) + " with unknown last name";
		else if (ln_h.size() > 0)
			return ln_h.back() + PrintHistory(ln_h) + " with unknown first name";
		else
			return "Incognito";
	}
private:
	map<int, string> firstName;
	map<int, string> lastName;
};
