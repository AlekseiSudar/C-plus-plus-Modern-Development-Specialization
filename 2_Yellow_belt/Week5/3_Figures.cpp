#include <iostream>
#include <sstream>
#include <iomanip> 
#include <string>
#include <vector>
#include <memory>
#include "math.h"
using namespace std;

const double Pi = 3.14;

class Figure {
public:
    Figure(const string& type) : type_(type) {}

    string Name() const { return type_; }

    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;

    const string type_;
};

class Triangle : public Figure {
public:
    Triangle(double a, double b, double c) 
        : Figure("TRIANGLE") 
        , a_(a), b_(b), c_(c) {}
    double Perimeter() const { return a_ + b_ + c_; }
    double Area() const {
        double p = Perimeter() / 2;
        return sqrt(p * (p - a_) * (p - b_) * (p - c_));
    }
private:
    double a_, b_, c_;
};

class Rect : public Figure {
public:
    Rect(double a, double b) 
        : Figure("RECT")
        , a_(a), b_(b) {}
    double Perimeter() const { return (a_ + b_) * 2; }
    double Area() const { return a_ * b_; }
private:
    double a_, b_;
};

class Circle : public Figure {
public:
    Circle(double r) 
        : Figure("CIRCLE")
        , r_(r) {}
    double Perimeter() const { return 2 * Pi * r_; }
    double Area() const { return Pi * r_ * r_; }
private:
    double r_;
};

shared_ptr<Figure> CreateFigure(istringstream& is) {
    string type;
    is >> type;
    if (type == "TRIANGLE") {
        double a, b, c;
        is >> a >> ws >> b >> ws >> c >> ws;
        return make_shared<Triangle>(a, b, c);
    }
    else if (type == "RECT") {
        double a, b;
        is >> a >> ws >> b >> ws;
        return make_shared<Rect>(a, b);
    }
    else if (type == "CIRCLE") {
        double r;
        is >> r >> ws;
        return make_shared<Circle>(r);
    }
}

int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line); ) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD") {
            // Пропускаем "лишние" ведущие пробелы.
            // Подробнее об std::ws можно узнать здесь:
            // https://en.cppreference.com/w/cpp/io/manip/ws
            is >> ws;
            figures.push_back(CreateFigure(is));
        }
        else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                cout << fixed << setprecision(3)
                    << current_figure->Name() << " "
                    << current_figure->Perimeter() << " "
                    << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}