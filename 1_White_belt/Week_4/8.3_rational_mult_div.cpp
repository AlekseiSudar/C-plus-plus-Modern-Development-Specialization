#include <iostream>
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

Rational operator+(const Rational& r, const Rational& l) {
    return Rational(
        r.Numerator() * l.Denominator() + l.Numerator() * r.Denominator(),
        r.Denominator() * l.Denominator()
    );
}
Rational operator-(const Rational& r, const Rational& l) {
    return Rational(
        r.Numerator() * l.Denominator() - l.Numerator() * r.Denominator(),
        r.Denominator()*l.Denominator()
    );
}
Rational operator*(const Rational& r, const Rational& l) {
    return Rational(
        r.Numerator() * l.Numerator(),
        r.Denominator() * l.Denominator()
    );
}
Rational operator/(const Rational& r, const Rational& l) {
    return Rational(
        r.Numerator() * l.Denominator(),
        r.Denominator() * l.Numerator()
    );
}

bool operator==(const Rational& r, const Rational& l) {
    if (r.Numerator() == l.Numerator())
        if (r.Denominator() == l.Denominator())
            return true;
    return false;
}