#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Human {
public:
    Human(const string& type, const string& name)
        : Type(type)
        , Name(name) {}

    virtual void Walk(const string& destination) const = 0;

    string strTypeName() const {
        return Type + ": " + Name;
    }
public:
    const  string Type;
    const string Name;
};

class Student : public Human {
public:

    Student(const string& name, const string& favouriteSong)
        : Human("Student", name)
        , FavouriteSong(favouriteSong) {}

    void Learn() const {
        cout << strTypeName() << " learns" << endl;
    }

    void Walk(const string& destination) const {
        cout << strTypeName() << " walks to: " << destination << endl;
        cout << strTypeName() << " sings a song: " << FavouriteSong << endl;
    }

    void SingSong() const {
        cout << strTypeName() << " sings a song: " << FavouriteSong << endl;
    }

public:
    const string FavouriteSong;
};


class Teacher : public Human {
public:

    Teacher(const string& name, const string& subject)
        : Human("Teacher", name)
        , Subject(subject) {}

    void Teach() const {
        cout << strTypeName() << " teaches: " << Subject << endl;
    }

    void Walk(const string& destination) const {
        cout << strTypeName() << " walks to: " << destination << endl;
    }

public:
    const string Subject;
};


class Policeman : public Human {
public:
    Policeman(const string& name)
        : Human("Policeman", name) {}


    void Check(const Human& h) const {
        cout << strTypeName() << " checks " << h.Type << ". " << h.Type <<"'s name is: " << h.Name << endl;
    }


    void Walk(const string& destination) const {
        cout << strTypeName() << " walks to: " << destination << endl;
    }
};


void VisitPlaces(const Human& h, vector<string> places) {
    for (const auto& p : places) {
        h.Walk(p);
    }
}

int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, { "Moscow", "London" });
    p.Check(s);
    VisitPlaces(s, { "Moscow", "London" });
    return 0;
}
