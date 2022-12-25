#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

struct Student {
    string first_name;
    string last_name;
    int day;
    int month;
    int year;
};

int main() {
    int num_of_student;
    cin >> num_of_student;
    vector<Student> students(num_of_student);
    for (auto& s : students) {
        cin >> s.first_name;
        cin >> s.last_name;
        cin >> s.day;
        cin >> s.month;
        cin >> s.year;
    }

    int num_of_comm;
    cin >> num_of_comm;
    for (int i = 0; i < num_of_comm; ++i) {
        string comm;
        int student_num;
        cin >> comm;
        cin >> student_num;

        if (student_num > 0 && student_num <= students.size())
        {
            if (comm == "name")
                cout <<
                students[student_num - 1].first_name << " " <<
                students[student_num - 1].last_name;
            else if (comm == "date")
                cout <<
                students[student_num - 1].day << '.' <<
                students[student_num - 1].month << '.' <<
                students[student_num - 1].year;
            else
                cout << "bad request";
        }
        else
            cout << "bad request";

        if (i != num_of_comm - 1)
            cout << endl;
    }
    
    

    return 0;
}
