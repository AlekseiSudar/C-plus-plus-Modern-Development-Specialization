#include <iostream>
#include <sstream>
#include <numeric>
using namespace std;

class Rational {
public:
    Rational() {
        numerator = 0;
        denominator = 1;
    }

    Rational(int num, int den) {
        if (den > 0) {
            int div = gcd(num, den);
            numerator = num / div;
            denominator = den / div;
        }
        else {
            int div = gcd(num, den);
            numerator = -num / div;
            denominator = -den / div;
        }
    }

    int Numerator() const {
        return numerator;
    }

    int Denominator() const {
        return denominator;
    }

private:
    int numerator, denominator;
};

bool operator==(const Rational& r, const Rational& l) {
    if (r.Numerator() == l.Numerator())
        if (r.Denominator() == l.Denominator())
            return true;
    return false;
}

ostream& operator<<(ostream& stream, const Rational& value) {
    stream << value.Numerator() << '/' << value.Denominator();
    return stream;
}
istream& operator>>(istream& stream, Rational& value) {
    int num, div;
    char d;
    if (stream >> num &&
        stream >> d &&
        stream >> div)
    {
        if (d == '/')
            value = Rational(num, div);
    }
    return stream;
}