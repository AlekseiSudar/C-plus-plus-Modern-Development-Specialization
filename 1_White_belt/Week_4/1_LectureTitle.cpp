#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Specialization {
    string data;
    explicit Specialization(string new_data) {
        data = new_data;
    }
};
struct Course {
    string data;
    explicit Course(string new_data) {
        data = new_data;
    }
};
struct Week {
    string data;
    explicit Week(string new_data) {
        data = new_data;
    }
};

struct LectureTitle {
    string specialization;
    string course;
    string week;
    LectureTitle(Specialization s, Course c, Week w) {
        specialization = s.data;
        course = c.data;
        week = w.data;
    }
};
