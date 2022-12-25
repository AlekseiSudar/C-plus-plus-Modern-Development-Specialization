#include <iostream>
#include <string>
#include <map>

using namespace std;

string FindName(map<int, string>& Name, int& year) {
    auto it = Name.upper_bound(year);
    string name;
    if (it != Name.begin())
        name = (--it)->second;
    return name;
}

class Person {
public:
    void ChangeFirstName(int year, const string& first_name) {
        FirstName[year] = first_name;
    }
    void ChangeLastName(int year, const string& last_name) {
        LastName[year] = last_name;
    }
    string GetFullName(int year) {
        string first_name = FindName(FirstName, year);
        string last_name = FindName(LastName, year);

        if (first_name.empty())
            if (last_name.empty())
                return "Incognito";
            else
                return last_name + " with unknown first name";
        else
            if (last_name.empty())
                return first_name + " with unknown last name";
            else
                return first_name + ' ' + last_name;
    }
private:
    map<int, string> FirstName;
    map<int, string> LastName;
};

int main() {
    Person person;

    person.ChangeFirstName(1965, "Polina");
    person.ChangeLastName(1967, "Sergeeva");
    for (int year : {1900, 1965, 1990}) {
        cout << person.GetFullName(year) << endl;
    }

    person.ChangeFirstName(1970, "Appolinaria");
    for (int year : {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
    }

    person.ChangeLastName(1968, "Volkova");
    for (int year : {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
    }

    return 0;
}
